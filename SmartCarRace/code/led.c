#include "zf_common_headfile.h"
#include "led.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化屏幕
// 参数说明     void
// 返回参数     void
// 使用示例     led_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void led_init()
{
    ips114_init();
    ips114_show_string(0, 0, "mt9v03x init.");
    while(1)
    {
        if(mt9v03x_init())
            ips114_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }
    ips114_show_string(0, 16, "init success.");
}
