#include "pch.h"
#include <Windows.h>

using namespace OpenHardwareMonitor::Hardware;
using namespace System;

int main(array<System::String ^> ^args)
{
	Computer^ computer = gcnew Computer(); // Classe "computer" do OpenHardwareMonitor instanciada.
	computer->CPUEnabled = true; // Ativação de CPU e GPU para monitoramento.
	computer->GPUEnabled = true;
	computer->Open(); // Inicia o monitoramento.
	
	for each (IHardware ^ hardware in computer->Hardware) {
		if (hardware->HardwareType == HardwareType::CPU || 
			hardware->HardwareType == HardwareType::GpuNvidia ||
			hardware->HardwareType == HardwareType::GpuAti) { // Verificação se o hardware é CPU ou GPU. Não pode ser utilizado Em vez de OU, pois um hardware não pode ser CPU e GPU ao mesmo tempo.
				hardware->Update();
				
				switch (hardware->HardwareType) { // switch case para checar o tipo de hardware e imprimir seu respectivo nome.
				case HardwareType::CPU:
					Console::WriteLine("CPU: ");
					break;
				case HardwareType::GpuNvidia:
					Console::WriteLine("GPU: ");
					break;
				case HardwareType::GpuAti:
					Console::WriteLine("GPU: ");
					break;
				default:
					Console::WriteLine("Erro ao Identificar CPU e GPU");
					break;
				}
				
				for each (ISensor ^ sensor in hardware->Sensors) { // Loop para imprimir os sensores de cada hardware.
					Console::WriteLine(sensor->Name + ": " + sensor->Value); // Imprime o nome do sensor e seu valor.
				}

				for each (hardware in hardware->SubHardware) { // Loop para imprimir os sub hardwares de cada hardware.
					hardware->Update();
					Console::WriteLine("SubHardware: " + hardware->Name); // Imprime o nome do sub hardware.
					
					for each (ISensor ^ sensor in hardware->Sensors) { // Loop para imprimir os sensores de cada sub hardware.
						Console::WriteLine(sensor->Name + ": " + sensor->Value); // Imprime o nome do sensor e seu valor.
					}
				}
	}
    return 0;
}
