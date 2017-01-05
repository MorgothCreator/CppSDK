#include <api/init.h>
#include<api/io_handle.h>
#include<api/timer.h>

int main(void)
{
    /*
     * Create a 500ms nonblocking timer.
     */
    GI::Sys::Timer blink_timer = GI::Sys::Timer(500);
    /*
     * Take controll to "led-0" pin.
     */
    GI::IO led_pin = GI::IO((char *)"led-0");
    while(1)
    {
        /*
         * Ceck if is a tick.
         */
        if(blink_timer.tick())
        {
            bool state;
            /*
             * Get "led-0" state.
             */
            led_pin.read(&state);
            if(state)
                /*
                 * If "led-0" state is '1' put it to '0'.
                 */
                led_pin.write(false);
            else
                /*
                 * If "led-0" state is '0' put it to '1'.
                 */
                led_pin.write(true);
        }
    }
}
