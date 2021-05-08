
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT   //小爱同学

#include <Blinker.h>

char auth[] = " ";  //这里密钥
char ssid[] = " ";   //这里WIFI名称
char pswd[] = " ";    //这里WIFI密码

BlinkerButton start_Button("start");     //定义按钮数据
BlinkerButton function_Button("function");
BlinkerButton close_Button("close");
BlinkerButton fast_Button("fast");
BlinkerButton dry_Button("dry_rice");
BlinkerButton porridge_Button("porridge");
BlinkerButton wet_Button("wet_rice");

bool dry_state = false;
bool porridge_state = false;
bool wet_state = false;
bool fast_state = false;

const int close_pin = 5;//D1
const int start_pin = 4;//D2
const int fast_pin = 14;//D5
const int function_pin = 13;//D7
 
BlinkerNumber FAST("fast");
BlinkerNumber PORRIDGE("porridge");
BlinkerNumber WET("wet");
BlinkerNumber DRY("dry");

bool oState = false;
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {     //小爱同学控制开命令
        digitalWrite(15, LOW);

        BlinkerMIOT.powerState("on");

        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {   //小爱同学控制关命令
        digitalWrite(15,HIGH);

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

void heartbeat()
{
    FAST.print(fast_state);
    PORRIDGE.print(porridge_state);
    WET.print(wet_state);
    DRY.print(dry_state);
}

void dataRead(const String & data)      // 如果未绑定的组件被触发，则会执行其中内容
{
    BLINKER_LOG("Blinker readString: ", data);
    Blinker.vibrate();
    uint32_t BlinkerTime = millis();
    Blinker.print("millis", BlinkerTime);
}

void start_callback(const String & state)     //点灯app内控制按键触发
{
    digitalWrite(start_pin,HIGH);
    delay(100);
    digitalWrite(start_pin,LOW);
    delay(100);
    digitalWrite(start_pin,HIGH);
    heartbeat();
}

void close_callback(const String & state)     //点灯app内控制按键触发
{
    digitalWrite(close_pin,HIGH);
    delay(100);
    digitalWrite(close_pin,LOW);
    delay(100);
    digitalWrite(close_pin,HIGH);
    dry_state = false;
    wet_state = false;
    porridge_state = false;
    fast_state = false;
    heartbeat();
}

void closeAllWork()
{
    BLINKER_LOG("Blinker dry_state: ", dry_state);
    BLINKER_LOG("Blinker wet_state: ", wet_state);
    BLINKER_LOG("Blinker porridge_state: ", porridge_state);
    BLINKER_LOG("Blinker fast_state: ", fast_state);
    if(dry_state|| wet_state || porridge_state || fast_state){
      close_callback("none");
      delay(500);
      dry_state = false;
      wet_state = false;
      porridge_state = false;
      fast_state = false;
    }
    heartbeat();
}

void function_callback(const String & state)     //点灯app内控制按键触发
{
    digitalWrite(function_pin,HIGH);
    delay(100);
    digitalWrite(function_pin,LOW);
    delay(100);
    digitalWrite(function_pin,HIGH);
    heartbeat();

void fast_callback(const String & state)     //点灯app内控制按键触发
{
    closeAllWork();
    digitalWrite(fast_pin,HIGH);
    delay(100);
    digitalWrite(fast_pin,LOW);
    delay(100);
    digitalWrite(fast_pin,HIGH);
    fast_state = !fast_state;
    BLINKER_LOG("fast_callback state:", fast_state);
    heartbeat();
}

void porridge_callback(const String & state)     //点灯app内控制按键触发
{

    closeAllWork();
    function_callback("none");
    delay(500);
    function_callback("none");
    delay(500);
    start_callback("none");
    porridge_state = !porridge_state;
    BLINKER_LOG("get button state:off", state);
    heartbeat();
}

void wet_callback(const String & state)     //点灯app内控制按键触发
{
    closeAllWork();
    function_callback("none");
    delay(500);
    start_callback("none");
    wet_state = !wet_state;
    BLINKER_LOG("get button state:on", state);
    heartbeat();
}

void dry_callback(const String & state)     //点灯app内控制按键触发
{
    closeAllWork();
    start_callback("none");
    dry_state = !dry_state;
    BLINKER_LOG("get button state:off", state);
    heartbeat();
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();

    pinMode(close_pin, OUTPUT);              //定义io口为输出
    digitalWrite(close_pin, HIGH);           //定义io默认为高电平
    pinMode(start_pin, OUTPUT);              //定义io口为输出
    digitalWrite(start_pin, HIGH);           //定义io默认为高电平
    pinMode(function_pin, OUTPUT);              //定义io口为输出
    digitalWrite(function_pin, HIGH);           //定义io默认为高电平
    pinMode(fast_pin, OUTPUT);              //定义io口为输出
    digitalWrite(fast_pin, HIGH);           //定义io默认为高电平

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);

    start_Button.attach(start_callback);
    function_Button.attach(function_callback);
    close_Button.attach(close_callback);
    fast_Button.attach(fast_callback);
    dry_Button.attach(dry_callback);
    porridge_Button.attach(porridge_callback);
    wet_Button.attach(wet_callback);
   
}

void loop()
{
    Blinker.run();
}
