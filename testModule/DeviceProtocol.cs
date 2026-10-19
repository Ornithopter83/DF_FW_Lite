using System.Globalization;

namespace DFTestModule;

internal static class DeviceProtocol
{
    public const int LedDimmingMaxPattern = 6;
    public const int LedDimmingDefaultPattern = 6;

    public static string Torque(int duty, int timeMs)
    {
        Range(duty, 0, 255, "메인 모터 Duty"); Range(timeMs, 0, 9999, "동작 시간");
        return timeMs == 0 ? string.Format(CultureInfo.InvariantCulture, "$02{0:000}%", duty)
            : string.Format(CultureInfo.InvariantCulture, "$02{0:000}{1:0000}%", duty, timeMs);
    }

    public static string Bldc(bool clockwise, int duty, int timeMs)
    {
        Range(duty, 0, 255, "BLDC Duty"); Range(timeMs, 0, 9999, "동작 시간");
        return string.Format(CultureInfo.InvariantCulture, "$04{0}{1:000}{2:0000}%", clockwise ? 1 : 2, duty, timeMs);
    }

    public static string TorqueOff() { return "$02000%"; }
    public static string BldcOff() { return "$0400000000%"; }
    public static string ReelVibration(bool on) { return on ? "$231%" : "$230%"; }
    public static string ReelLeftLed(bool on) { return on ? "$2411%" : "$2410%"; }
    public static string ReelRightLed(bool on) { return on ? "$2421%" : "$2420%"; }
    public static string Imu(bool on) { return on ? "$081%" : "$080%"; }
    public static string EncoderCalibrationStart() { return "$37S%"; }
    public static string EncoderCalibrationComplete() { return "$37C%"; }
    public static string EncoderCalibrationApply() { return "$37A%"; }
    public static string EncoderCalibrationCancel() { return "$37X%"; }
    public static string EncoderCalibrationQuery() { return "$37Q%"; }
    public static string LedControl(int position, int action, int color, int timeMs)
    {
        Range(position, 0, 3, "LED 위치"); Range(action, 0, 3, "LED 표시 액션");
        if (action == 3) Range(color, 1, LedDimmingMaxPattern, "디밍 패턴");
        else Range(color, 0, 15, "LED 색상");
        Range(timeMs, 10, 9999, "LED 시간");
        if (action == 0) return string.Format(CultureInfo.InvariantCulture, "$06{0}0%", position);
        if (action == 1) return string.Format(CultureInfo.InvariantCulture, "$06{0}1{1:00}%", position, color);
        return string.Format(CultureInfo.InvariantCulture, "$06{0}{1}{2:00}{3:0000}%", position, action, color, timeMs);
    }

    public static string LedBrightnessRead() { return "$0608%"; }
    public static string LedBrightnessWrite(int value) { Range(value, 10, 100, "LED 밝기"); return string.Format(CultureInfo.InvariantCulture, "$0609{0:000}%", value); }

    public static string InputMonitor(string input, bool on)
    {
        string code = input switch {
            "부트" => "01-00", "릴" => "05-00", "메인" => "03-00", "메인엔코더" => "03-03",
            _ => throw new ArgumentException("알 수 없는 입력 대상입니다.", nameof(input))
        };
        return "$27" + code + "," + (on ? "1" : "0") + "%";
    }

    private static void Range(int value, int min, int max, string name)
    {
        if (value < min || value > max) throw new ArgumentOutOfRangeException(name);
    }
}





