int main()
{
    can_init("can0");

    while(true)
    {
        send_rpm(rpm);

        rpm += 100;

        if(rpm > 8000)
            rpm = 800;

        usleep(50000);
    }
}
