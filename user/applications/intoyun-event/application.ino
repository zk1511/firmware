/*
 * InotYun 测试程序
 */

//SerialUSBDebugOutput debugOutput(115200, ALL_LEVEL);
//SerialDebugOutput debugOutput(115200, ALL_LEVEL);

PRODUCT_ID(y4NFFyDE9uq6H202)     //产品ID
PRODUCT_SECRET(ab697b0dc1716d24cfc49b071668e766) //产品密钥
PRODUCT_VERSION(2)     //产品版本号

#define DPID_ENUM_LIGHT_MODE             1        //颜色模式
#define DPID_NUMBER_TEMPERATURE          2        //温度
#define DPID_BOOL_SWITCH                 3        //灯泡开关
#define DPID_BOOL_LIGHT_STATUS           4        //灯泡亮灭状态
#define DPID_NUMBER_RHEOSTAT             5        //速度
#define DPID_STRING_LCD_DISPLAY          6        //字符显示
#define DPID_BINARY_DATA                 7        //透传数据

#define LEDPIN    LED_BUILTIN    //例子灯

static enum eDeviceState
{
    DEVICE_STATE_INIT,
    DEVICE_STATE_JOIN,
    DEVICE_STATE_SEND,
    DEVICE_STATE_SLEEP,
    DEVICE_STATE_IDLE,
}deviceState;

uint32_t prevTime = 0;
bool sleepEnable = false;

int LightMode;
double Temperature;
bool LightSwitch;
bool LightStatus;
int Rheostat_tmp;
int Rheostat;
String LcdDisplay;
uint8_t userBuffer[256];

void system_event_callback(system_event_t event, int param, uint8_t *data, uint16_t datalen)
{
    Serial.printf("system_event_callback");
    if((event == event_cloud_data)){
        if(param == ep_cloud_data_datapoint){
            // 颜色模式
            if (RESULT_DATAPOINT_NEW == IntoRobot.readDatapoint(DPID_ENUM_LIGHT_MODE, LightMode)){
                Serial.printf("Read Light Mode: ");
                switch(LightMode)
                {
                case 0:
                    Serial.printf("RED");
                    break;
                case 1:
                    Serial.printf("BLUE");
                    break;
                case 2:
                    Serial.printf("YELLOW");
                    break;
                default:
                    Serial.printf("Error");
                    break;
                }
                Serial.printf("\r\n");
            }
            //灯泡控制
            if (RESULT_DATAPOINT_NEW == IntoRobot.readDatapoint(DPID_BOOL_SWITCH, LightSwitch)){
                Serial.printf("switch: %d\r\n", LightSwitch);
                if(true == LightSwitch)
                {
                    digitalWrite(LEDPIN, LOW);     // 打开灯泡
                    IntoRobot.writeDatapoint(DPID_BOOL_LIGHT_STATUS, true);
                }
                else
                {
                    digitalWrite(LEDPIN, HIGH);    // 关闭灯泡
                    IntoRobot.writeDatapoint(DPID_BOOL_LIGHT_STATUS, false);
                }
            }

            //速度控制
            if (RESULT_DATAPOINT_NEW == IntoRobot.readDatapoint(DPID_NUMBER_RHEOSTAT, Rheostat_tmp)){
                Rheostat = Rheostat_tmp;
                Serial.printf("Speed: %d\r\n", Rheostat);
            }

            //字符串显示
            if (RESULT_DATAPOINT_NEW == IntoRobot.readDatapoint(DPID_STRING_LCD_DISPLAY, LcdDisplay)){
                Serial.printf("Lcd Display: %s\r\n", LcdDisplay.c_str());
            }
        }else if(param == ep_cloud_data_custom){
            memcpy( userBuffer, data, datalen );
        }
    }
}

static void SystemWakeUpHandler(void)
{
    if(Serial.isEnabled())
    {
        Serial.end();
    }
    Serial.begin(115200);
    Serial.println("mcuWakeup");
    DEBUG("sync word = 0x%x\r\n",SX1276Read(0x39));
    if(IntoYun.connected()){
        deviceState = DEVICE_STATE_SEND;
    }else{
        deviceState = DEVICE_STATE_JOIN;
    }
}

void lorawan_event_callback(system_event_t event, int param, uint8_t *data, uint16_t datalen)
{
    switch(event)
    {
        case event_lorawan_status:
            switch(param)
            {
                case ep_lorawan_join_success: //入网成功
                    DEBUG("lorawan event joined ok\r\n");
                    break;

                case ep_lorawan_join_fail: //入网失败
                    DEBUG("lorawan event joined fail\r\n");
                    break;

                case ep_lorawan_send_success:
                    DEBUG("lorawan event send success\r\n");
                    break;

                case ep_lorawan_send_fail:
                    DEBUG("lorawan event send fail\r\n");
                    break;

                default:
                    break;
            }
            break;

            default:
                break;
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(LEDPIN, OUTPUT);
    IntoRobot.defineDatapointEnum(DPID_ENUM_LIGHT_MODE, DP_PERMISSION_UP_DOWN, 0);                          //颜色模式
    IntoRobot.defineDatapointNumber(DPID_NUMBER_TEMPERATURE, DP_PERMISSION_UP_ONLY, -100, 100, 2, 0);       //温度
    IntoRobot.defineDatapointBool(DPID_BOOL_SWITCH, DP_PERMISSION_UP_DOWN, false);                          //灯泡开关
    IntoRobot.defineDatapointBool(DPID_BOOL_LIGHT_STATUS, DP_PERMISSION_UP_ONLY, false);                    //灯泡亮灭状态
    IntoRobot.defineDatapointNumber(DPID_NUMBER_RHEOSTAT, DP_PERMISSION_UP_DOWN, 0, 1000, 0, 0);            //速度
    IntoRobot.defineDatapointString(DPID_STRING_LCD_DISPLAY, DP_PERMISSION_UP_DOWN, 255, "oh yeah!");       //字符显示
    IntoRobot.defineDatapointBinary(DPID_BINARY_DATA, DP_PERMISSION_UP_DOWN, 255, "\x23\x32\x32\x43", 4);   //字符显示
    System.on(event_lorawan_status, &lorawan_event_callback);
    System.on(event_cloud_data, &system_event_callback);
    LoRaWan.setDataRate(DR_3);
    LoRaWan.setChannelDRRange(2,DR_3,DR_3);
    LoRaWan.setRX2Params(DR_3,434665000);
    LoRaWan.setChannelStatus(0,false);
    LoRaWan.setChannelStatus(1,false);
    deviceState = DEVICE_STATE_JOIN;
    uint32_t delayMs = (uint32_t)random(0,10000);
    DEBUG("delayMs = %d\r\n",delayMs);
    delay(delayMs);
    IntoYun.connect(JOIN_OTAA,0);
}

uint8_t cnt = 0;
void loop()
{
    #if  1
    switch(deviceState)
    {
    case DEVICE_STATE_JOIN:
        // IntoYun.connect(JOIN_OTAA,0);
        deviceState = DEVICE_STATE_IDLE;
        break;

    case DEVICE_STATE_SEND:
        if(Temperature > 100){
            Temperature = 0;
        }else{
            Temperature += 0.1;
        }
        IntoRobot.writeDatapoint(DPID_NUMBER_TEMPERATURE, Temperature);
        //速度上送
        if(Rheostat >= 1000){
            Rheostat = 0;
        }else{
            Rheostat += 5;
        }
        IntoRobot.writeDatapoint(DPID_NUMBER_RHEOSTAT, Rheostat);
        if(IntoRobot.sendDatapointAll(false,50) == 0){
            DEBUG("send frame ok\r\n");
            deviceState = DEVICE_STATE_SLEEP;
        }else{
            DEBUG("send frame fail\r\n");
            deviceState = DEVICE_STATE_SLEEP;
        }
        break;

    case DEVICE_STATE_IDLE:
        if(IntoYun.connected() == 0){
            LoRaWan.setMacClassType(CLASS_C);
            deviceState = DEVICE_STATE_SEND;
        }
        break;

    case DEVICE_STATE_SLEEP:
        if(LoRaWan.getMacClassType() == CLASS_C){
            delay(60000);
            deviceState = DEVICE_STATE_SEND;
        }else{
            System.sleep(SystemWakeUpHandler,20); //定时唤醒
        }
        break;
    default:
        break;
    }
    #endif

    #if 0 
    if(LoRaWan.connected())
    {
        //温度上送
        if(Temperature > 100){
            Temperature = 0;
        }else{
            Temperature += 0.1;
        }
        IntoRobot.writeDatapoint(DPID_NUMBER_TEMPERATURE, Temperature);
        //速度上送
        if(Rheostat >= 1000){
            Rheostat = 0;
        }else{
            Rheostat += 5;
        }
        IntoRobot.writeDatapoint(DPID_NUMBER_RHEOSTAT, Rheostat);
        if(IntoRobot.sendDatapointAll(false,10))
        {
            DEBUG("sendDatapointAll ok\r\n");
        }
        else
        {
            DEBUG("send confirm frame fail\r\n");
        }
        delay(30000);
    }
    #endif

    #if 0
    switch(deviceState)
    {
        case DEVICE_STATE_INIT:
                break;

        case DEVICE_STATE_JOIN:
            DEBUG("lorawan joinOTAA\r\n");
            LoRaWan.joinOTAA(2);
            deviceState = DEVICE_STATE_IDLE;
            break;

        case DEVICE_STATE_SEND:
            //温度上送
            if(Temperature > 100){
                Temperature = 0;
            }else{
                Temperature += 0.1;
            }
            IntoRobot.writeDatapoint(DPID_NUMBER_TEMPERATURE, Temperature);
            //速度上送
            if(Rheostat >= 1000){
                Rheostat = 0;
            }else{
                Rheostat += 5;
            }
            IntoRobot.writeDatapoint(DPID_NUMBER_RHEOSTAT, Rheostat);
            IntoRobot.sendDatapointAll();
            deviceState = DEVICE_STATE_IDLE;
            break;

        case DEVICE_STATE_IDLE:
            break;

        case DEVICE_STATE_SLEEP:
            delay(20000);
            deviceState = DEVICE_STATE_SEND;
            // System.sleep(SystemWakeUpHandler,10); //定时唤醒
            break;

        default:
            break;
    }

    if(sleepEnable)
    {
        if(millis() - prevTime >= 3000)
        {
            sleepEnable = false;
            deviceState = DEVICE_STATE_SLEEP;
        }
    }
    #endif

    #if 0 
    //温度上送
    if(Temperature > 100){
        Temperature = 0;
    }else{
        Temperature += 0.1;
    }
    IntoRobot.writeDatapoint(DPID_NUMBER_TEMPERATURE, Temperature);
    //速度上送
    if(Rheostat >= 1000){
        Rheostat = 0;
    }else{
        Rheostat += 5;
    }
    IntoRobot.writeDatapoint(DPID_NUMBER_RHEOSTAT, Rheostat);
    IntoRobot.sendDatapointAll();
    delay(10000);
    #endif

}

