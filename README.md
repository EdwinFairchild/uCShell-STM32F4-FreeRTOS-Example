# uCShell-STM32F4-FreeRTOS-Example

## Important 
This repo uses [Git Submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) so if you download the zip file provided by github you will not get all the files necessary,
specifically the uCShell source files. 

If using HTTPS clone using this command:

`git clone https://github.com/EdwinFairchild/uCShell-STM32F4-FreeRTOS-Example.git --recurse-submodules`

If using SSH clone with this command:

`git clone git@github.com:EdwinFairchild/uCShell-STM32F4-FreeRTOS-Example.git --recurse-submodules`


### Description
Absolutely nothiing out of this world going on here. This is just a boiler plate example for someone to base
their STM32 project using uCShell and freeRTOS in conjunction. 

### Hardware

While uCShell itself is hardware agnostic, this example project uses an STM32F446RE Nucelo and STM32Cube so  porting it to another STM32 should be no problem.

### Software
You just need STM32Cube Ide to open up the project and flash it onto the Nucleo.
Furthermore this is an example using freeRTOS to make a uCShell task and a blink LED task.
