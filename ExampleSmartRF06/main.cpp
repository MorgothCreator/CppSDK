#include <api/init.h>
#include <stdio.h>

#include <api/timer.h>
#include <api/io_handle.h>

#include <sys/core_init.h>

#include <app/sys/cmd.h>

int main(void)
{
	GI::Sys::Timer blink_timer = GI::Sys::Timer(100);

	/* Get control of "led-0" pin.*/
	GI::IO led_pin = GI::IO((char *)"led-0");

	Cmd term = Cmd((char *)"uart-0", (char *)"uart-0", (char *)"uart-0");

	while(1)
	{
		//GI::Sys::Clock::sleep();
		dev.idle();
		term.idle();
		if(blink_timer.tick())
		{
			bool state;
			led_pin.read(&state);
			if(state)
				led_pin.write(false);
			else
				led_pin.write(true);
		}
	}
}
