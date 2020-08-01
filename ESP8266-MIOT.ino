
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT   //小爱同学
#define BLINKER_DUEROS_OUTLET
#define BLINKER_ALIGENIE_OUTLET //天猫精灵

#include <Blinker.h>

char auth[] = "3dd9da6f37a2";  //这里密钥
char ssid[] = "CMCC-LnR7";   //这里WIFI名称
char pswd[] = "7mg2zwsq";    //这里WIFI密码
BlinkerButton Button1("btn-on");     //定义按钮数据
BlinkerButton Button2("btn-off"); 
bool oState = false;
int counter = 0;
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {     //小爱同学控制开命令
        digitalWrite(0, LOW);

        BlinkerMIOT.powerState("on");

        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {   //小爱同学控制关命令
        digitalWrite(0,HIGH);

        BlinkerMIOT.powerState("off");

        BlinkerMIOT.print();

        oState = false;
    }
}

void miotQuery(int32_t queryCode)      //小爱同学控制
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
}

void AliGeniePowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {     //小爱同学控制开命令
        digitalWrite(0, LOW);

        BlinkerAliGenie.powerState("on");

        BlinkerAliGenie.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {   //小爱同学控制关命令
        digitalWrite(0,HIGH);

        BlinkerAliGenie.powerState("off");

        BlinkerAliGenie.print();

        oState = false;
    }
}

void AliGenieQuery(int32_t queryCode)      //天猫精灵控制
{
    BLINKER_LOG("AliGenie Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("AliGenie Query All");
            BlinkerAliGenie.powerState(oState ? "on" : "off");
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("AliGenie Query Power State");
            BlinkerAliGenie.powerState(oState ? "on" : "off");
            BlinkerAliGenie.print();
            break;
        default :
            BlinkerAliGenie.powerState(oState ? "on" : "off");
            BlinkerAliGenie.print();
            break;
    }
}

void dataRead(const String & data)      // 如果未绑定的组件被触发，则会执行其中内容
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();

    uint32_t BlinkerTime = millis();

    Blinker.print("millis", BlinkerTime);
}

void button1_callback(const String & state)     //点灯app内控制按键触发
{

    digitalWrite(0,LOW);
    BLINKER_LOG("get button state:on", state);


}
void button2_callback(const String & state)     //点灯app内控制按键触发
{

    digitalWrite(0,HIGH);
    BLINKER_LOG("get button state:off", state);


}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
     BLINKER_DEBUG.debugAll();

    pinMode(0, OUTPUT);              //定义io口为输出
    digitalWrite(0, LOW);           //定义io默认为高电平

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    

    BlinkerAliGenie.attachPowerState(AliGeniePowerState);  
    BlinkerAliGenie.attachQuery(AliGenieQuery);
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
    Button1.attach(button1_callback);  
    Button2.attach(button2_callback);     
   
}

void loop()
{
    Blinker.run();
}
