/*
 * InotYun 测试程序
 */

//SerialUSBDebugOutput debugOutput(115200, ALL_LEVEL);
//SerialDebugOutput debugOutput(115200, ALL_LEVEL);

PRODUCT_ID(PctOaj94F4KyHdkC3f4xrvWpEFRfj18f)     //产品ID
PRODUCT_SECRET(efacbc991dbdc66cbb327bcf04a1c119) //产品密钥
PRODUCT_VERSION(2)     //产品版本号

#define DPID_ENUM_LIGHT_MODE             1        //颜色模式
#define DPID_NUMBER_TEMPERATURE          2        //温度
#define DPID_BOOL_SWITCH                 3        //灯泡开关
#define DPID_BOOL_LIGHT_STATUS           4        //灯泡亮灭状态
#define DPID_NUMBER_RHEOSTAT             5        //速度
#define DPID_STRING_LCD_DISPLAY          6        //字符显示

#define LEDPIN    LED_BUILTIN    //例子灯

int LightMode;
double Temperature;
bool LightSwitch;
bool LightStatus;
int Rheostat_tmp;
int Rheostat;
String LcdDisplay;

void setup()
{
    Serial.begin(115200);
    pinMode(LEDPIN, OUTPUT);
    IntoRobot.defineDatapointEnum(DPID_ENUM_LIGHT_MODE, DP_PERMISSION_UP_DOWN, 0);                     //颜色模式
    IntoRobot.defineDatapointNumber(DPID_NUMBER_TEMPERATURE, DP_PERMISSION_UP_ONLY, -100, 100, 2, 0);     //温度
    IntoRobot.defineDatapointBool(DPID_BOOL_SWITCH, DP_PERMISSION_UP_DOWN, false);                     //灯泡开关
    IntoRobot.defineDatapointBool(DPID_BOOL_LIGHT_STATUS, DP_PERMISSION_UP_ONLY, false);               //灯泡亮灭状态
    IntoRobot.defineDatapointNumber(DPID_NUMBER_RHEOSTAT, DP_PERMISSION_UP_DOWN, 0, 100, 0, 0);        //速度
    IntoRobot.defineDatapointString(DPID_STRING_LCD_DISPLAY, DP_PERMISSION_UP_DOWN, "oh yeah!");       //字符显示
}

void loop()
{
    /*
    IntoRobot.writeDatapoint(DPID_ENUM_LIGHT_MODE, 1);
    IntoRobot.writeDatapoint(DPID_NUMBER_TEMPERATURE, 11.11);
    IntoRobot.writeDatapoint(DPID_BOOL_SWITCH, true);
    IntoRobot.writeDatapoint(DPID_BOOL_LIGHT_STATUS, true);
    IntoRobot.writeDatapoint(DPID_NUMBER_RHEOSTAT, 343);
    IntoRobot.writeDatapoint(DPID_STRING_LCD_DISPLAY, "adfb");
    IntoRobot.sendDatapointAll();
    delay(10000);
    */

        // 颜色模式
    if (RESULT_DATAPOINT_NEW == IntoRobot.readDatapoint(DPID_ENUM_LIGHT_MODE, LightMode))
    {
        Serial.printf("Read Light Mode: ");
        switch(LightMode)
        {
            case 0:
                Serial.printf("GREEN");
                break;
            case 1:
                Serial.printf("RED");
                break;
            case 2:
                Serial.printf("BLUE");
                break;
            default:
                Serial.printf("Error");
                break;
        }
        Serial.printf("\r\n");
        IntoRobot.sendDatapoint(DPID_ENUM_LIGHT_MODE, LightMode);
    }
    //灯泡控制
    if (RESULT_DATAPOINT_NEW == IntoRobot.readDatapoint(DPID_BOOL_SWITCH, LightSwitch))
    {
        if(true == LightSwitch)
        {
#if PLATFORM_ID == PLATFORM_ATOM || PLATFORM_ID == PLATFORM_NEUTRON
            digitalWrite(LEDPIN, HIGH);    // 打开灯泡
#else
            digitalWrite(LEDPIN, LOW);     // 打开灯泡
#endif
            IntoRobot.sendDatapoint(DPID_BOOL_LIGHT_STATUS, true);
        }
        else
        {
#if PLATFORM_ID == PLATFORM_ATOM || PLATFORM_ID == PLATFORM_NEUTRON
            digitalWrite(LEDPIN, LOW);     // 关闭灯泡
#else
            digitalWrite(LEDPIN, HIGH);    // 关闭灯泡
#endif
            IntoRobot.sendDatapoint(DPID_BOOL_LIGHT_STATUS, false);
        }
        IntoRobot.sendDatapoint(DPID_BOOL_SWITCH, LightSwitch);
    }

    //速度控制
    if (RESULT_DATAPOINT_NEW == IntoRobot.readDatapoint(DPID_NUMBER_RHEOSTAT, Rheostat_tmp))
    {
        Rheostat = Rheostat_tmp;
        Serial.printf("Speed: %d\r\n", Rheostat);
    }

    //字符串显示
    if (RESULT_DATAPOINT_NEW == IntoRobot.readDatapoint(DPID_STRING_LCD_DISPLAY, LcdDisplay))
    {
        Serial.printf("Lcd Display: %s\r\n", LcdDisplay.c_str());
        IntoRobot.sendDatapoint(DPID_STRING_LCD_DISPLAY, LcdDisplay);
    }

    //温度上送
    if(Temperature > 100)
    {Temperature = 0;}
    else
    {Temperature += 0.1;}
    IntoRobot.writeDatapoint(DPID_NUMBER_TEMPERATURE, Temperature);

    //速度上送
    if(Rheostat > 100)
    {Rheostat = 0;}
    else
    {Rheostat += 5;}
    IntoRobot.writeDatapoint(DPID_NUMBER_RHEOSTAT, Rheostat);

    IntoRobot.sendDatapointAll();

    delay(10000);

}

