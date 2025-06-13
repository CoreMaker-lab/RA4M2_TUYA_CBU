#include "hal_data.h"
#include <stdio.h>
FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER


/* UART0——涂鸦模块：单字节接收完成标志 */
volatile bool uart_wifi_RX_flag = false;
/* UART0——涂鸦模块：发送完成标志*/
volatile bool uart_wifi_TX_flag = false;
/* UART0 接收缓存大小*/
#define UART0_LENGTH  255
/* UART0 接收环形缓冲区*/
uint8_t  TUYA_wifi_buffer[UART0_LENGTH];
/* 已接收字节计数（环形缓存写指针）*/
uint32_t UART0_TUYA_LENGTH = 0;
/* 解析到一帧有效数据后置位，主循环里解析后需清零 */
uint32_t UART0_TUYA_flag = 0;

/*****************************************************************
 * UART0 回调：RX/TX/单字符事件处理
 *****************************************************************/
void user_uart_callback0(uart_callback_args_t *p_args)
{
    /* 整帧接收完成（需在 R_SCI_UART_Receive 调用后才会产生）*/
    if(p_args->event == UART_EVENT_RX_COMPLETE)
    {
        uart_wifi_RX_flag = true;
    }
    /* 发送完成 */
    else if(p_args->event == UART_EVENT_TX_COMPLETE)
    {
        uart_wifi_TX_flag = true;
    }
    /* 接收到 1 个字符*/
    else if(p_args->event == UART_EVENT_RX_CHAR)
    {
        /* 缓冲未满则写入*/
        if (sizeof(TUYA_wifi_buffer) > UART0_TUYA_LENGTH)
        {
            /* 仅在数据位 ≥ 8bit 时写入 1 字节 */
            if (UART_DATA_BITS_8 >= g_uart0_cfg.data_bits)
            {
                TUYA_wifi_buffer[UART0_TUYA_LENGTH++] = (uint8_t) p_args->data;
            }
            /* —— 以下为简单的涂鸦帧快速判定逻辑，可改为状态机 —— */
            /* 帧头非 0x55 则丢弃当前字节（回溯 1 位）*/
            if(TUYA_wifi_buffer[00]!=0x55)
                UART0_TUYA_LENGTH--;
            /* 普通指令帧长度 ≥ 7 且 CMD ≠ 0x07 时，即可认为一帧完 */
            if(UART0_TUYA_LENGTH>=7 && TUYA_wifi_buffer[3]!=0x06)
                UART0_TUYA_flag=1;
            /* CMD = 0x06（DP 下发）需根据数据点判断长度*/
            if(TUYA_wifi_buffer[3]==0x06)
            {
                /* DP-ID 0x65：亮度，数据长度 0x0004 -> 总长 ≥ 15 */
                if(TUYA_wifi_buffer[6]==0x65 && UART0_TUYA_LENGTH>=15)//亮度值
                    UART0_TUYA_flag=1;
                /* DP-ID 0x66：开关，数据长度 0x0001 -> 总长 ≥ 12 */
                else if(TUYA_wifi_buffer[6]==0x66 && UART0_TUYA_LENGTH>=12)//开关
                    UART0_TUYA_flag=1;
            }
        }
        }
}

/*****************************************************************
 * 与涂鸦协议相关的工作变量／固定协议帧
 * *****************************************************************/

uint8_t wifi_first =0;/* 0：第一次心跳；1：第二次心跳     */
uint32_t wifi_num =0;//如果心跳频繁发送，可能是触发了复位，需要重新发送buff1，这里2秒内多次发送心跳指令则认为重启
/* ---- 固定格式下行帧：MCU 主动发送给涂鸦模块 ----------------- */
const uint8_t g_tuya_heartbeat1[8]={0x55,0xAA,0x03,0x00,0x00,0x01,0x00,0x03};//心跳检测,第1次 0x55 aa 00 00 00 01 00 03
const uint8_t g_tuya_heartbeat2[8]={0x55,0xAA,0x03,0x00,0x00,0x01,0x01,0x04};//心跳检测,第2次 0x55 aa 00 00 00 01 01 04
const uint8_t g_tuya_wifi_cfg[8]={0x55,0xAA,0x03,0x05,0x00,0x01,0x01,0x09};//WIFI配网
//0x55, 0xAA, 0x03, 0x01 (帧头)
//0x00, 0x2A       (长度)
//0x7B, 0x22, 0x70, 0x22, 0x3A, 0x22 ({"p":")
//0x78, 0x68, 0x6E, 0x7A, 0x74, 0x79, 0x64, 0x67, 0x77, 0x64, 0x63, 0x67, 0x6B, 0x71, 0x78, 0x64 (xhnztydgwdcgkqxd)
//0x22, 0x2C, 0x22, 0x76, 0x22, 0x3A, 0x22 (","v":")
//0x31, 0x2E, 0x30, 0x2E, 0x30  (1.0.0)
//0x22, 0x2C, 0x22, 0x6D, 0x22, 0x3A (","m":)
//0x30  (0)
//0x7D(})
//0xC(校验码)
uint8_t g_tuya_product_info[49]={
    0x55, 0xAA, 0x03, 0x01,
    0x00, 0x2A,
    0x7B, 0x22, 0x70, 0x22, 0x3A, 0x22,
    0x78, 0x68, 0x6E, 0x7A, 0x74, 0x79, 0x64, 0x67, 0x77, 0x64, 0x63, 0x67, 0x6B, 0x71, 0x78, 0x64,
    0x22, 0x2C, 0x22, 0x76, 0x22, 0x3A, 0x22,
    0x31, 0x2E, 0x30, 0x2E, 0x30,
    0x22, 0x2C, 0x22, 0x6D, 0x22, 0x3A,
    0x30,
    0x7D,
    0xCC

};//接收模块发送的查询产品信息请求

const uint8_t g_tuya_query_mode[8]={0x55,0xaa,0x03,0x02,0x00,0x00,0x04};//查询工作模式
uint32_t wifi_ap_num =0;//wifi重置计时
const uint8_t g_tuya_rpt_net[8]={0x55,0xaa,0x03,0x03,0x00,0x00,0x05};//报告设备联网状态

uint32_t wifi_Update=0;//wifi发送标志位
uint32_t wifi_Rx=0;//wifi接收指令标志位
uint32_t LED_PWM_num=10;//亮度值
/* DP-ID 0x65：亮度值 */
uint8_t g_tuya_dp_pwm[15]={0x55,0xAA,0x03,0x07,0x00,0x08,0x65,0x02,0x00,0x04,0x00,0x00,0x00,0x0A,0x86};//MCU亮度
bool LED_open=0; /* LED 开关：0=OFF，1=ON                   */
/* DP-ID 0x66：开关   */
uint8_t g_tuya_dp_switch[12]={0x55,0xAA,0x03,0x07,0x00,0x05,0x66,0x01,0x00,0x01,0x00,0x76};//MCU上报开关状态

/** @brief 由中断回调函数切换的外部中断标志 */
static volatile bool s_ext_irq_flag = false;
bool button_flag =0;
/**
 * @brief 外部中断 IRQ4 回调函数
 *
 * 当 ICU 外部中断发生时调用，切换外部中断标志。
 *
 * @param[in] p_args 中断回调参数（未使用）
 */
void external_irq4_callback(external_irq_callback_args_t *p_args)
{
    (void)p_args;
    s_ext_irq_flag = !s_ext_irq_flag;
    button_flag=1;
    LED_open=!LED_open;
}

/**
 * @brief 初始化外部中断 IRQ4
 *
 * 打开并使能 ICU 外部中断通道 4。
 *
 * @return FSP_SUCCESS 表示成功，否则返回错误码
 */
static fsp_err_t external_irq4_init(void)
{
    fsp_err_t err = R_ICU_ExternalIrqOpen(&g_external_irq4_ctrl, &g_external_irq4_cfg);
    if (FSP_SUCCESS != err)
    {
        return err;
    }

    err = R_ICU_ExternalIrqEnable(&g_external_irq4_ctrl);
    return err;
}

/**
 * @brief 根据中断标志更新 LED 输出
 *
 * 根据 s_ext_irq_flag 状态设置指定 IOPORT 引脚高低电平。
 */
static inline void led_update(void)
{
//    R_IOPORT_PinWrite(&g_ioport_ctrl,
//                      BSP_IO_PORT_01_PIN_04,
//                      s_ext_irq_flag ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);
    if(button_flag)
    {
        button_flag=0;

        wifi_Update=1;
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_04, LED_open);

    }
}


/** @brief PWM 呼吸灯当前占空比计数（0-1000） */
static uint32_t s_pwm_count = 0;
/** @brief PWM 呼吸灯占空比变化方向：true=递增，false=递减 */
static bool s_pwm_increasing = true;

/**
 * @brief 初始化 GPT2 作为 PWM 呼吸灯
 *
 * 配置定时器周期、启动并默认占空比 0。
 * @return FSP_SUCCESS 成功，否则返回错误码
 */
static fsp_err_t pwm_breathe_init(void)
{
    fsp_err_t err = R_GPT_Open(&g_timer2_ctrl, &g_timer2_cfg);
    if (FSP_SUCCESS != err) return err;

    err = R_GPT_PeriodSet(&g_timer2_ctrl, 9000);
    if (FSP_SUCCESS != err) return err;

    /* 启动定时器产生 PWM */
    err = R_GPT_Start(&g_timer2_ctrl);
    if (FSP_SUCCESS != err) return err;

    /* 初始占空比设置为 0（熄灭） */
    return R_GPT_DutyCycleSet(&g_timer2_ctrl, 0, GPT_IO_PIN_GTIOCA);
}

/**
 * @brief 更新 PWM 占空比实现呼吸效果
 *
 * 根据当前计数和方向计算新占空比并写入 GPT。
 */
static void pwm_breathe_update(void)
{
    /* 调整占空比计数 */
    if (s_pwm_increasing)
    {
        s_pwm_count++;
        if (s_pwm_count >= 1000)
        {
            s_pwm_count = 1000;
            s_pwm_increasing = false;
        }
    }
    else
    {
        s_pwm_count--;
        if (s_pwm_count == 0)
        {
            s_pwm_increasing = true;
        }
    }

    /* 设置新的比较值：周期 9000 -> 1% = 90 */
    (void)R_GPT_DutyCycleSet(&g_timer2_ctrl,
                              s_pwm_count * 9,
                              GPT_IO_PIN_GTIOCA);
}

/** @brief UART 发送完成标志 */
volatile bool uart_send_complete_flag = false;
/*******************************************************************************************************************//**
 * @brief UART 发送回调
 *
 * 当 UART 发送完成时调用，置位发送完成标志。
 **********************************************************************************************************************/
void user_uart_callback (uart_callback_args_t * p_args)
{
    if(p_args->event == UART_EVENT_TX_COMPLETE)
    {
        uart_send_complete_flag = true;
    }
}

/*******************************************************************************************************************//**
 * @brief 串口重定向函数
 **********************************************************************************************************************/
#ifdef __GNUC__                                 //串口重定向
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif


PUTCHAR_PROTOTYPE
{
    fsp_err_t err = R_SCI_UART_Write(&g_uart9_ctrl, (uint8_t *)&ch, 1);
        if(FSP_SUCCESS != err) __BKPT();
        while(uart_send_complete_flag == false){}
        uart_send_complete_flag = false;
        return ch;
}

int _write(int fd,char *pBuffer,int size)
{
    for(int i=0;i<size;i++)
    {
        __io_putchar(*pBuffer++);
    }
    return size;
}


/*******************************************************************************************************************
 * @brief 向涂鸦模块上报最新 DP 数据 & 本地硬件同步
 *******************************************************************************************************************/
void tuya_wifi_Update(void)
{
    if(wifi_Update)
    {
        printf("wifi_Update\n");
        wifi_Update=0;
        /*更新 GPT 占空比 ---------------------------------------*/
        /* 计数器周期在 FSP 配置为 9000（举例），
           因此 1% 亮度 = 9000 / 1000 = 9 个计数。                  */
        fsp_err_t err = R_GPT_DutyCycleSet(&g_timer2_ctrl,
                                 9000-LED_PWM_num * 9,           // 新比较值
                                 GPT_IO_PIN_GTIOCA);  // 使用 A 通道
        assert(FSP_SUCCESS == err);
        /* 更新 DP 帧 ------------------------------------------------------*/
        g_tuya_dp_pwm[12]=LED_PWM_num>>8;
        g_tuya_dp_pwm[13]=LED_PWM_num;
        g_tuya_dp_pwm[14]=0x86-0x0A+g_tuya_dp_pwm[12]+g_tuya_dp_pwm[13];

        /*发送 DP-亮度 */
        printf("[MCU] -> TUYA DP_PWM = %d\r\n",LED_PWM_num);
        err = R_SCI_UART_Write(&g_uart0_ctrl, g_tuya_dp_pwm, 15);
        if(FSP_SUCCESS != err) __BKPT();
        while(uart_wifi_TX_flag == false){}
        uart_wifi_TX_flag = false;

        /*发送 DP-开关 */
        printf("[MCU] -> TUYA DP_SW=%d\r\n",LED_open);
        g_tuya_dp_switch[10]=LED_open;
        g_tuya_dp_switch[11]=0x76+g_tuya_dp_switch[10];
        //上报LED开关
        err = R_SCI_UART_Write(&g_uart0_ctrl, g_tuya_dp_switch, 12);
        if(FSP_SUCCESS != err) __BKPT();
        while(uart_wifi_TX_flag == false){}
        uart_wifi_TX_flag = false;

    }

}

/*******************************************************************************************************************
 * @brief 长按按键 3 s 进入配网模式 (发送 0x05 命令)
 *******************************************************************************************************************/
void button_wifi_ap(void)
{
    //  wifi_ap_num
    bsp_io_level_t p_port_value_pin_111;
    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_11, &p_port_value_pin_111);
    if(p_port_value_pin_111)/* 松开 */
        wifi_ap_num=0;
    else//长按3s复位wifi
    {
        if(wifi_ap_num<3000)
            wifi_ap_num++;
        else if(wifi_ap_num==3000)
        {
            wifi_ap_num++;
            printf("[BTN] wifi_ap_mode\r\n");
            fsp_err_t err = R_SCI_UART_Write(&g_uart0_ctrl, g_tuya_wifi_cfg, 8);
            if(FSP_SUCCESS != err) __BKPT();
            while(uart_wifi_TX_flag == false){}
            uart_wifi_TX_flag = false;
        }
    }
}

/*******************************************************************************************************************
 * @brief 解析完成后，根据 CMD 打印清晰友好的日志
 *******************************************************************************************************************/
void uart0_tuya(void)
{
    if(UART0_TUYA_flag ==1)//接收完成标志
    {

        fsp_err_t err = FSP_SUCCESS;
        UART0_TUYA_flag=0;
        if(TUYA_wifi_buffer[0]==0x55&&TUYA_wifi_buffer[1]==0xAA)//判断帧头和版本
        {
            if(TUYA_wifi_buffer[3]==0x00)//判断是否为心跳检测
            {
                printf("[TUYA] <heartbeat(SEQ=%u)\r\n", wifi_first);
//                if(wifi_num<2000&&wifi_first==1)//频繁发送心跳指令，认为重启
//                {
//                    wifi_first=0;
//                   }
//                wifi_num=0;
                if(wifi_first==0)//第一次发送心跳数据
                {
                    wifi_first=1;
                    //心跳检测，向涂鸦模块发送
                    err = R_SCI_UART_Write(&g_uart0_ctrl, g_tuya_heartbeat1, 8);
                    if(FSP_SUCCESS != err) __BKPT();
                    while(uart_wifi_TX_flag == false){}
                    uart_wifi_TX_flag = false;
                    }
                else
                {
                    //心跳检测，向涂鸦模块发送
                    err = R_SCI_UART_Write(&g_uart0_ctrl, g_tuya_heartbeat2, 8);
                    if(FSP_SUCCESS != err) __BKPT();
                    while(uart_wifi_TX_flag == false){}
                    uart_wifi_TX_flag = false;
                }
            }
            else if(TUYA_wifi_buffer[3]==0x01)//接收模块发送的查询产品信息请求
            {
                printf("[TUYA] <Query Product Information\r\n");
                //接收模块发送的查询产品信息请求，向涂鸦模块发送
                err = R_SCI_UART_Write(&g_uart0_ctrl, g_tuya_product_info, 49);
                if(FSP_SUCCESS != err) __BKPT();
                while(uart_wifi_TX_flag == false){}
                uart_wifi_TX_flag = false;
            }
            else if(TUYA_wifi_buffer[3]==0x02)//查询工作模式
            {
                printf("[TUYA] <Query Work Mode\r\n");
                //查询工作模式，向涂鸦模块发送
                err = R_SCI_UART_Write(&g_uart0_ctrl, g_tuya_query_mode, 8);
                if(FSP_SUCCESS != err) __BKPT();
                while(uart_wifi_TX_flag == false){}
                uart_wifi_TX_flag = false;
            }
            else if(TUYA_wifi_buffer[3]==0x03)//报告设备联网状态
            {
                printf("[TUYA] <WIFI_MODE=%02X\r\n", TUYA_wifi_buffer[6]);
                //查询工作模式，向涂鸦模块发送
                err = R_SCI_UART_Write(&g_uart0_ctrl, g_tuya_rpt_net, 8);
                if(FSP_SUCCESS != err) __BKPT();
                while(uart_wifi_TX_flag == false){}
                uart_wifi_TX_flag = false;

                if(TUYA_wifi_buffer[6]==0x04)//已连上路由器且连接到云端
                {
                    wifi_Update=1;//wifi跟新标志位
                }
            }
            else if(TUYA_wifi_buffer[3]==0x06)//接收模块DP下发信息
            {

                if(TUYA_wifi_buffer[6]==0x66)//LED开关
                {
                    wifi_Update=1;//wifi跟新标志位
                    LED_open=TUYA_wifi_buffer[10];
                    button_flag=1;
                    printf("[TUYA] <DP-switch=%s\r\n", LED_open ? "ON" : "OFF");
                }
                else if(TUYA_wifi_buffer[6]==0x65)//LED亮度值
                {

                    wifi_Update=1;//wifi跟新标志位
                    LED_PWM_num=0;
                    LED_PWM_num+=TUYA_wifi_buffer[13];
                    LED_PWM_num+=TUYA_wifi_buffer[12]*256;
                    printf("[TUYA] <DP-PWM=%ld\r\n", LED_PWM_num);
                }
            }
        }
        // 清除数组
        memset(TUYA_wifi_buffer, 0, UART0_LENGTH);
        // 同时把当前有效长度归零
        UART0_TUYA_LENGTH = 0;
    }
}


/*******************************************************************************************************************//**
 * main() is generated by the RA Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry(void)
{
    /* TODO: add your own code here */

    /* 初始化外部中断 IRQ4 */
    fsp_err_t err = external_irq4_init();
    assert(FSP_SUCCESS == err);

    /* 初始化 PWM 呼吸灯 */
    err = pwm_breathe_init();
    assert(FSP_SUCCESS == err);

    /* 打开 UART9 实例 */
    err = R_SCI_UART_Open(&g_uart9_ctrl, &g_uart9_cfg);
    assert(FSP_SUCCESS == err);

    unsigned char buff[]="RA E2STUDIO";
    uint8_t buff_len = strlen(buff);
    err = R_SCI_UART_Write(&g_uart9_ctrl, buff, buff_len);
    if(FSP_SUCCESS != err) __BKPT();
        while(uart_send_complete_flag == false){}
    uart_send_complete_flag = false;

    printf("\nhello world!\r\n");

    /* Open the transfer instance with initial configuration. */
    err = R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    assert(FSP_SUCCESS == err);

    /* 主循环 */
    while (true)
    {
        led_update();             /* 切换普通 LED */
//        pwm_breathe_update();     /* 呼吸灯效果 */
        tuya_wifi_Update(); /* 涂鸦数据上报 */
        button_wifi_ap();   /* 配网按键检测 */
        uart0_tuya();       /* 涂鸦协议解析 */
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }

#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open (&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);

#if BSP_CFG_SDRAM_ENABLED

        /* Setup SDRAM and initialize it. Must configure pins first. */
        R_BSP_SdramInit(true);
#endif
    }
}

#if BSP_TZ_SECURE_BUILD

FSP_CPP_HEADER
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ();

/* Trustzone Secure Projects require at least one nonsecure callable function in order to build (Remove this if it is not required to build). */
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ()
{

}
FSP_CPP_FOOTER

#endif
