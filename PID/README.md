# HAL库注意事项

## 初始化
    1.HAL库初始化函数应该放在SystemClock_Config();MX_GPIO_Init();MX_TIM1_Init();之后，先等待时钟和GPIO完成配置在初始化，但是在标准库中是手动开启时钟则不需要考虑这些问题

## 定时器中运行函数时间测量问题
    1.在使用定时器实时获取数据更新时候，会遇到定时器中执行的程序超过定时时间，对于项目数据获取不利
```c
    //标准库测量程序运行时间
    void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        //调用TIM2进行计时
        uint16_t start = TIM_GetCounter(TIM2);

        Key_loop();

        //读取TIM2结束计数值
        uint16_t end = TIM_GetCounter(TIM2);

        //计算耗时
        uint16_t elapsed;
        if (end >= start)
            elapsed = end - start;
        else
            elapsed = (65535 - start) + end + 1;

        //转为微秒（此处 1 计数 = 1us）
        uint16_t time_us = elapsed;  // 如果预分频配成 1us/计数

        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

```

```c
    //HAL库测量程序运行时间
    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        uint16_t start, end, elapsed;

        // 1. 读取 TIM2 开始计数
        start = __HAL_TIM_GET_COUNTER(&htim2);

        Key_loop();

        // 2. 读取 TIM2 结束计数
        end = __HAL_TIM_GET_COUNTER(&htim2);

        // 3. 计算耗时，考虑 TIM2 溢出
        if (end >= start)
            elapsed = end - start;
        else
            elapsed = (65535 - start) + end + 1;

        uint16_t time_us = elapsed;  // 如果预分频配成 1us/计数

        g_key_loop_time_us = elapsed;
    }
}



```