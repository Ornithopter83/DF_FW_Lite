using System.Drawing;

namespace DFTestModule;

// Stage 1: 최소 제어/수신 창. 장비 통합 검증 및 기능 축소는 추후 작업.
internal sealed class MainForm : Form
{
    private readonly SerialConnection serial = new();
    private readonly FrameDecoder decoder = new();
    private readonly ComboBox portBox = new() { Width = 110, DropDownStyle = ComboBoxStyle.DropDownList };
    private readonly Button connectionButton = new() { Text = "연결", AutoSize = true };
    private readonly Label connectionLabel = new() { Text = "연결 안 됨", AutoSize = true };
    private readonly Label rodLabel = new() { Text = "Rod: ?", AutoSize = true };
    private readonly Label imuLabel = new() { Text = "IMU: ?", AutoSize = true };
    private readonly Label versionLabel = new() { Text = "버전: ?", AutoSize = true };
    private readonly Label inputLabel = new() { Text = "입력: ?", AutoSize = true };
    private readonly NumericUpDown torqueDuty = new() { Minimum = 0, Maximum = 255, Value = 60, Width = 70 };
    private readonly NumericUpDown bldcDuty = new() { Minimum = 0, Maximum = 255, Value = 60, Width = 70 };
    private readonly TextBox manualCommand = new() { Width = 160, Text = "$10%" };
    private readonly RichTextBox log = new() {
        ReadOnly = true, Dock = DockStyle.Fill, Font = new Font("Consolas", 9),
        BackColor = Color.White, WordWrap = false
    };
    private readonly System.Windows.Forms.Timer pollTimer = new() { Interval = 2000 };

    public MainForm()
    {
        Text = "DF Lite Test Module — Stage 1";
        MinimumSize = new Size(760, 560);
        ClientSize = new Size(960, 660);
        StartPosition = FormStartPosition.CenterScreen;
        Font = new Font("맑은 고딕", 9F);
        BuildUi();
        serial.BytesReceived += OnBytesReceived;
        pollTimer.Tick += (_, _) => {
            if (!serial.IsOpen || !TrySend("$00%", false)) pollTimer.Stop();
        };
        FormClosing += (_, _) => { pollTimer.Stop(); serial.Dispose(); pollTimer.Dispose(); };
        RefreshPorts();
    }

    private void BuildUi()
    {
        var root = new TableLayoutPanel {
            Dock = DockStyle.Fill, ColumnCount = 1, RowCount = 5,
            Padding = new Padding(12)
        };
        for (int i = 0; i < 4; ++i) root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
        root.RowStyles.Add(new RowStyle(SizeType.Percent, 100));

        var connectRow = NewRow();
        AddText(connectRow, "포트");
        connectRow.Controls.Add(portBox);
        AddButton(connectRow, "재검색", RefreshPorts);
        connectionButton.Click += async (_, _) => await ToggleConnectionAsync();
        connectRow.Controls.Add(connectionButton);
        connectRow.Controls.Add(connectionLabel);

        var motorGroup = new GroupBox { Text = "Main 모터 (1초 제한 구동)", Height = 95, Dock = DockStyle.Top };
        var motorRow = NewRow();
        motorRow.Dock = DockStyle.Fill;
        AddText(motorRow, "Torque Duty");
        motorRow.Controls.Add(torqueDuty);
        AddButton(motorRow, "Torque 1초", () => TrySend(DeviceProtocol.Torque((int)torqueDuty.Value, 1000)));
        AddButton(motorRow, "Torque 정지", () => TrySend(DeviceProtocol.TorqueOff()));
        AddText(motorRow, "BLDC Duty");
        motorRow.Controls.Add(bldcDuty);
        AddButton(motorRow, "정방향 1초", () => TrySend(DeviceProtocol.Bldc(true, (int)bldcDuty.Value, 1000)));
        AddButton(motorRow, "역방향 1초", () => TrySend(DeviceProtocol.Bldc(false, (int)bldcDuty.Value, 1000)));
        AddButton(motorRow, "BLDC 정지", () => TrySend(DeviceProtocol.BldcOff()));
        motorGroup.Controls.Add(motorRow);

        var rodGroup = new GroupBox { Text = "Rod 기본 출력", Height = 87, Dock = DockStyle.Top };
        var rodRow = NewRow();
        rodRow.Dock = DockStyle.Fill;
        AddButton(rodRow, "진동 ON", () => TrySend(DeviceProtocol.ReelVibration(true)));
        AddButton(rodRow, "진동 OFF", () => TrySend(DeviceProtocol.ReelVibration(false)));
        AddButton(rodRow, "왼쪽 LED ON", () => TrySend(DeviceProtocol.ReelLeftLed(true)));
        AddButton(rodRow, "왼쪽 LED OFF", () => TrySend(DeviceProtocol.ReelLeftLed(false)));
        AddButton(rodRow, "오른쪽 LED ON", () => TrySend(DeviceProtocol.ReelRightLed(true)));
        AddButton(rodRow, "오른쪽 LED OFF", () => TrySend(DeviceProtocol.ReelRightLed(false)));
        AddButton(rodRow, "IMU ON", () => TrySend(DeviceProtocol.Imu(true)));
        AddButton(rodRow, "IMU OFF", () => TrySend(DeviceProtocol.Imu(false)));
        rodGroup.Controls.Add(rodRow);

        var statusRow = NewRow();
        statusRow.Controls.Add(rodLabel);
        statusRow.Controls.Add(imuLabel);
        statusRow.Controls.Add(versionLabel);
        statusRow.Controls.Add(inputLabel);
        statusRow.Controls.Add(manualCommand);
        AddButton(statusRow, "명령 전송", SendManualCommand);
        AddButton(statusRow, "모터 정지", () => _ = StopMotorsAsync());

        root.Controls.Add(connectRow, 0, 0);
        root.Controls.Add(motorGroup, 0, 1);
        root.Controls.Add(rodGroup, 0, 2);
        root.Controls.Add(statusRow, 0, 3);
        root.Controls.Add(log, 0, 4);
        Controls.Add(root);
    }

    private static FlowLayoutPanel NewRow() => new() {
        FlowDirection = FlowDirection.LeftToRight, WrapContents = true,
        AutoSize = true, Padding = new Padding(4), Margin = new Padding(0, 3, 0, 3)
    };

    private static void AddText(FlowLayoutPanel row, string label) =>
        row.Controls.Add(new Label { Text = label, AutoSize = true, Margin = new Padding(6, 9, 6, 3) });

    private static void AddButton(FlowLayoutPanel row, string label, Action action)
    {
        var button = new Button { Text = label, AutoSize = true, Margin = new Padding(4) };
        button.Click += (_, _) => action();
        row.Controls.Add(button);
    }

    private void RefreshPorts()
    {
        var previous = portBox.SelectedItem?.ToString();
        portBox.Items.Clear();
        portBox.Items.AddRange(SerialConnection.FindPorts());
        if (previous != null && portBox.Items.Contains(previous)) portBox.SelectedItem = previous;
        else if (portBox.Items.Count > 0) portBox.SelectedIndex = 0;
    }

    private async Task ToggleConnectionAsync()
    {
        if (serial.IsOpen) {
            await StopMotorsAsync();
            Disconnect();
            return;
        }
        if (portBox.SelectedItem == null) { MessageBox.Show("연결할 COM 포트를 선택하세요."); return; }
        connectionButton.Enabled = false;
        try {
            decoder.Reset();
            serial.Open(portBox.SelectedItem.ToString()!);
            connectionButton.Text = "연결 해제";
            connectionLabel.Text = serial.PortName + " 초기화 중";
            Append("INFO", "시리얼 연결됨 (115200bps)");
            await InitializeMainAsync();
            if (serial.IsOpen && !IsDisposed) {
                connectionLabel.Text = serial.PortName + " 연결됨";
                pollTimer.Start();
            }
        }
        catch (Exception ex) {
            Append("ERROR", ex.Message);
            Disconnect();
        }
        finally { if (!IsDisposed) connectionButton.Enabled = true; }
    }

    // 원본 TM의 단계별 초기 명령 및 60/500ms 간격을 보존한다.
    private async Task InitializeMainAsync()
    {
        string[][] groups = {
            new[] { "$00%", "$290101%", "$1101%" },
            new[] { "$00%", "$080%" },
            new[] { "$00%", "$0112%", "$0110%", "$0115%", "$0117%", "$1501%" }
        };
        foreach (string[] group in groups) {
            foreach (string command in group) {
                if (!serial.IsOpen || IsDisposed || !TrySend(command, false)) return;
                await Task.Delay(60);
            }
            await Task.Delay(500);
        }
    }

    private async Task StopMotorsAsync()
    {
        if (!serial.IsOpen) return;
        TrySend(DeviceProtocol.TorqueOff(), false);
        await Task.Delay(80);
        if (serial.IsOpen) TrySend(DeviceProtocol.BldcOff(), false);
    }

    private void Disconnect()
    {
        pollTimer.Stop();
        serial.Close();
        connectionButton.Text = "연결";
        connectionLabel.Text = "연결 안 됨";
        rodLabel.Text = "Rod: ?";
        imuLabel.Text = "IMU: ?";
    }

    private void SendManualCommand()
    {
        string command = manualCommand.Text.Trim();
        if (command.Length < 3 || command.Length > 120 || command[0] != '$'
            || command[^1] != '%' || command.Any(c => c > 127)) {
            MessageBox.Show("ASCII $...% 형식이며 120자 이하여야 합니다.");
            return;
        }
        TrySend(command);
    }

    private bool TrySend(string command, bool showError = true)
    {
        if (!serial.IsOpen) {
            if (showError) MessageBox.Show("먼저 연결하세요.");
            return false;
        }
        try { serial.WriteText(command); Append("TX", command); return true; }
        catch (Exception ex) {
            Append("ERROR", ex.Message);
            if (showError) MessageBox.Show(ex.Message, "송신 실패");
            return false;
        }
    }

    private void OnBytesReceived(byte[] data)
    {
        foreach (string frame in decoder.Push(data)) {
            try {
                if (IsHandleCreated && !IsDisposed)
                    BeginInvoke(new Action(() => ProcessFrame(frame)));
            }
            catch (InvalidOperationException) { /* 창 종료 중 */ }
        }
    }

    private void ProcessFrame(string frame)
    {
        if (IsDisposed) return;
        Append("RX", frame);
        if (frame.StartsWith("$20", StringComparison.Ordinal) && frame.Length >= 5)
            rodLabel.Text = "Rod: " + (frame.Substring(3, 2) == "01" ? "연결" : "미연결");
        else if (frame.StartsWith("$21", StringComparison.Ordinal) && frame.Length >= 5)
            imuLabel.Text = "IMU: " + (frame.Substring(3, 2) == "01" ? "연결" : "미연결");
        else if (frame.StartsWith("$10", StringComparison.Ordinal))
            versionLabel.Text = "버전: " + frame.Substring(3).TrimEnd('%');
        else if (frame.StartsWith("$13", StringComparison.Ordinal) || frame.StartsWith("$14", StringComparison.Ordinal)
            || frame.StartsWith("$09", StringComparison.Ordinal) || frame.StartsWith("$12", StringComparison.Ordinal))
            inputLabel.Text = "입력: " + frame.Substring(0, Math.Min(40, frame.Length));
        else if (frame == "$1800%")
            Append("WARN", "Main의 AP 종료 요청: TM Lite는 ACK/PC 종료를 자동 수행하지 않습니다.");
    }

    private void Append(string direction, string message)
    {
        if (IsDisposed) return;
        if (log.TextLength > 40000) log.Clear();
        log.AppendText($"[{DateTime.Now:HH:mm:ss}] {direction} {message}\r\n");
        log.SelectionStart = log.TextLength;
        log.ScrollToCaret();
    }
}
