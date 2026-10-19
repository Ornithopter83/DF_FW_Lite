using System.Globalization;

namespace DFTestModule;

// Main/Rod의 기존 wire format을 그대로 사용한다. Lite 신규 PID를 추가하지 않는다.
internal static class DeviceProtocol
{
    public static string Torque(int duty, int timeMs)
    {
        Range(duty, 0, 255);
        Range(timeMs, 1, 9999);
        return string.Format(CultureInfo.InvariantCulture, "$02{0:000}{1:0000}%", duty, timeMs);
    }

    public static string TorqueOff() => "$02000%";

    public static string Bldc(bool clockwise, int duty, int timeMs)
    {
        Range(duty, 0, 255);
        Range(timeMs, 1, 9999);
        return string.Format(CultureInfo.InvariantCulture, "$04{0}{1:000}{2:0000}%", clockwise ? 1 : 2, duty, timeMs);
    }

    public static string BldcOff() => "$0400000000%";
    public static string ReelVibration(bool on) => on ? "$231%" : "$230%";
    public static string ReelLeftLed(bool on) => on ? "$2411%" : "$2410%";
    public static string ReelRightLed(bool on) => on ? "$2421%" : "$2420%";
    public static string Imu(bool on) => on ? "$081%" : "$080%";

    private static void Range(int value, int minimum, int maximum)
    {
        if (value < minimum || value > maximum)
            throw new ArgumentOutOfRangeException(nameof(value));
    }
}
