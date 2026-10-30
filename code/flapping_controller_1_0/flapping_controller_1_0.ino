#include <Servo.h>

Servo servo_A0; // 定义舵机A0
Servo servo_A1; // 定义舵机A1
int datachan[10]; // 数组用来保存遥控器接收值
int x,y,chx; // 遥控接收程序中用到的变量
int channel1; // 通道1
int channel2; // 通道2
int channel3; // 通道3
int channel4; // 通道4
int channel5; // 通道5
int channel6; // 通道6
int channel7; // 通道7
int channel8; // 通道8
int pulsewidth_0; // 舵机控制函数，舵机的脉冲宽500-2500
int pulsewidth_1; // 舵机控制函数，舵机的脉冲宽500-2500
int servoC_0; // 舵机A0中间值，一般在1500左右
int servoC_1; // 舵机A1中间值，一般在1500左右
// 幅度、偏角、差速、升降；c5、c6 用于微调舵机初始位置
// sj0 是油门档做了升降的混控，sji 是差速转弯做的升降补偿
int pj,cs,sj,c5,c6,sj0,sj1,mag;
int hx; // 滑翔角度
float fd,ys,fd0; // 延时即油门，fd0 是油门档做了幅度的混控
int fly = 0; // 起飞标志位

void dataget(); // 获取ppm函数
void do_sv(); // 舵机控制函数

void do_sv()
{
    servo_A0.writeMicroseconds(pulsewidth_0);
    servo_A1.writeMicroseconds(pulsewidth_1); // 通过控制脉冲宽度来控制舵机的角度
}

void setup() // 初始化
{
    //Serial.begin(115200); // 开启后可进行串口调试
    servo_A0.attach(A0); // 舵机端口A0
    servo_A1.attach(A1); // 舵机端口A1
    servoC_0 = 1500; // 舵机A0角度初始值
    servoC_1 = 1500; // 舵机A1角度初始值
    fd = 600; // 幅度初始值600
    ys = 10000; // 延时初始值10000，单位微秒
    pj = 0; // 偏角初始0
    cs = 0; // 差速初始0
    sj = 0; // 升降初始0

    pulsewidth_0 = servoC_0; // 舵机A0脉冲宽度设为初始值
    pulsewidth_1 = servoC_1; // 舵机A0脉冲宽度设为初始值
    servo_A0.writeMicroseconds(pulsewidth_0); // 输出相应脉冲宽度控制舵机A0
    servo_A1.writeMicroseconds(pulsewidth_1); // 输出相应脉冲宽度控制舵机A1
    delay(1000); // 等待三秒
}

void loop() // 主程序重复执行
{
    dataget(); // 采集接收机返回值
    channel1 = datachan[0]; // 将1通道返回值保存在变量channel1
    channel2 = datachan[1]; // 将2通道返回值保存在变量channel2
    channel3 = datachan[2]; // 将3通道返回值保存在变量channel3
    channel4 = datachan[3]; // 将4通道返回值保存在变量channel4
    channel5 = datachan[4]; // 将5通道返回值保存在变量channel5
    channel6 = datachan[5]; // 将6通道返回值保存在变量channel6
    channel7 = datachan[6]; // 将71通道返回值保存在变量channel7
    channel8 = datachan[7];

    // 映射通道四的值用于控制偏角左倾或右倾，此处仿生蝴蝶不用，扑翼机可以用
    // pj = map(channel4,595,1595,-250,250);

    // 映射通道2的值给变量sj，用于控制蝴蝶升降，范围是-80到80
    sj = map(channel2,595,1595,80,-80);
    if (abs(sj) < 5) sj = 0;
    fd0 = map(channel3,595,1595,80,100); // 降低油门幅度会降至80%
    sj0 = map(channel3,595,1595,30,-30); // 油门升降混控
    if (abs(sj0) > 27) sj0 = 30;
    // 映射通道3的值给变量ys，用于控制蝴蝶扑翼频率，范围是10000到8500
    ys = map(channel3,595,1595,10000,8500);
    // 映射通道1的值给cs，控制两侧幅度差异，可以转向，范围-100到100
    cs = map(channel1,595,1595,-100,100);
    if (abs(cs) < 5) cs = 0;
    sj1 = map(abs(cs),0,100,0,25); // sji是差速转弯做的升降补偿
    // 映射通道5的值给变量c5，用于控制A0舵机初始位置
    c5 = map(channel5,595,1595,0,300);
    // 映射通道6的值给变量c6，用于控制A1舵机初始位置
    c6 = map(channel6,595,1595,0,300);
    servoC_0 = 1500 + c5; // 通过旋钮来微调舵机A0初始位置
    servoC_1 = 1500 - c6; // 通过旋钮来微调舵机A1初始位置

    // 通道7为三挡，分别控制三种幅度，分别是500、550、600
    if (channel7 > 1200)
        fd = 500*fd0/100;
    else if (700 < channel7 && channel7 < 1200)
        fd = 550*fd0/100;
    else if (channel7 < 700)
        fd = 600*fd0/100;

    if (channel8 > 1000)
        mag = 13;
    else
        mag = 1;

    // 当通道3在最下端时映射ys的值为11000左右，这时候fly为0；
    // 当ys小于9000时fly为1，启动
    if (ys > 9900)
        fly = 0;
    else if (ys < 8600)
        fly = 1;

    // fly为0时不启动；达到接近最高点8500时，以最高速启动，即快速启动
    if (!fly)
{
        pulsewidth_0 = servoC_0 - sj*mag - sj0 - sj1 - cs*2;
        // 不启动时只能通过通道2控制升降，cs在静止时控制倾角
        pulsewidth_1 = servoC_1 + sj*mag + sj0 + sj1 - cs*2;
        // 舵机按照上面的值进行执行
        do_sv();
}

    if (fly) // fly为1时启动
{
        // 将扑翼的半个周期分为18份，分别是cos0、cos10、cos20……cos170
        // y = Acos(wx) + h，其中A控制余弦函数的幅度，h控制余弦函数的0点位置
        for (int i = 0; i < 18; i++)
{
            pulsewidth_0 = (servoC_0 - sj - sj0 - sj1)
- (fd + cs)*cos((10*i)/180.0*3.14);
            pulsewidth_1 = (servoC_1 + sj + sj0 + sj1)
+ (fd - cs)*cos((10*i)/180.0*3.14);
            // 将计算好的数值即脉冲宽度给舵机，执行相应的角度
            do_sv();
            // ys越小，半个周期的时间越短，速度就越快，实现了通道3控制频率
            delayMicroseconds(ys);
}
        for (int i = 18; i > 0; i--)
{
            pulsewidth_0 = (servoC_0 - sj - sj0 - sj1)
- (fd + cs)*cos((10*i)/180.0*3.14);
            pulsewidth_1 = (servoC_1 + sj + sj0 + sj1)
+ (fd - cs)*cos((10*i)/180.0*3.14);
            do_sv();
            delayMicroseconds(ys);
}
}

    //Serial.println(pj); // 偏角
    //Serial.println(ys); // 延时
    //Serial.println(fd); // 幅度
    //Serial.println(cs); // 差速
    //Serial.println(sj); // 升降
    //Serial.println(hx); // 滑翔角度
    //Serial.println(pulsewidth_0);
}

void dataget()
{
    // 发送8个脉冲信号以后有一个结束脉冲，结束脉冲较大，一般大于5000；
    // 接下来依次是一到八通道的脉冲，数值大概是580到1400
    while (pulseIn(6,HIGH) < 5000) {}
    for (x = 0; x < 8; x++)
        datachan[x] = pulseIn(6,HIGH); // 将这8个脉冲分别保存在数组中
}
