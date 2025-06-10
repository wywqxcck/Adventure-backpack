#include "stm32f10x.h"                  // Device header
#include "app.h"

int main(void)
{
	app_init();
	while (1)
	{
			app_run();
	}
}

 