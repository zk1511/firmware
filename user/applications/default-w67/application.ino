/*
 * nut 默认程序
 */

//SerialUSBDebugOutput debugOutput(115200, ALL_LEVEL);
//SerialDebugOutput debugOutput(115200, ALL_LEVEL);

//PRODUCT_MODE(PRODUCT_MODE_SLAVE)   //模块处于主模式还是从模式
PRODUCT_ID(KEmyV460)   //产品ID
PRODUCT_SECRET(3e9c9ead45dfbe01e6d178a22f826588) //产品密钥
PRODUCT_VERSION(2)     //产品版本号

#define SMARTLIGHT_CMD_SWITCH    "channel/smartLight_0/cmd/switch"   //开关命令
#define SMARTLIGHT_DATA_STATUS   "channel/smartLight_0/data/status"  //开关状态

#define LEDPIN    GPIO16    //定义灯泡控制引脚

void smartLightSwitchCb(uint8_t *payload, uint32_t len)
{
    if(payload[0] == '1')
    {
        digitalWrite(LEDPIN, LOW);     //nut 低电平 打开灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"1");
    }
    else
    {
        digitalWrite(LEDPIN, HIGH);      //nut 高电平 关闭灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"0");
    }
}

void systemConfigKeyDeal()
{
    if(SYSTEM_CONFIG_TYPE_IMLINK_SERIAL != System.configStatus())
    {
        System.configBegin(SYSTEM_CONFIG_TYPE_IMLINK_SERIAL);
        digitalWrite(GPIO16, LOW);
    }
    else
    {
        System.configEnd();
        digitalWrite(GPIO16, HIGH);
    }
}

void setup()
{
    //初始化
    pinMode(LEDPIN, OUTPUT);
    pinMode(GPIO16, OUTPUT);
    attachInterrupt(GPIO0, systemConfigKeyDeal, CHANGE);
    //接收灯开关命令
    IntoRobot.subscribe(SMARTLIGHT_CMD_SWITCH, NULL, smartLightSwitchCb);
}

void loop()
{
}

