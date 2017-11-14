
void user_init(void)
{
    uart_set_baud(0, 115200);
    xTaskCreate(blinkenTask, "blinkenTask", 256, NULL, 2, NULL);
    //xTaskCreate(blinkenRegisterTask, "blinkenRegisterTask", 256, NULL, 2, NULL);
}
