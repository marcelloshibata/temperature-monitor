#include "pch.h"
#include <Windows.h>

using namespace LibreHardwareMonitor::Hardware;
using namespace System;

int main(array<System::String ^> ^args)
{
	SetConsoleOutputCP(CP_UTF8); // Configura a saída do console para UTF-8 para suportar caracteres acentuados (como Graus Celsius)

	Computer^ computer = gcnew Computer();
	computer->IsCpuEnabled = true; // Inicia o monitoramento de CPU
	computer->IsGpuEnabled = true; // Inicia o monitoramento de GPU
	computer->Open();

	while (true) {
		Console::Clear(); // Limpa a tela do console para atualizar os dados em tempo real

		for each (IHardware ^ hardware in computer->Hardware) {
			if (hardware->HardwareType == HardwareType::Cpu || // Condições que verificam se o hardware é do tipo CPU ou GPU, não é possivel usar E pois o hardware pode ser de um tipo ou de outro, mas não ambos ao mesmo tempo
				hardware->HardwareType == HardwareType::GpuAmd ||
				hardware->HardwareType == HardwareType::GpuIntel ||
				hardware->HardwareType == HardwareType::GpuNvidia) {
				hardware->Update(); // Atualiza os dados do hardware

				switch (hardware->HardwareType) {
				case HardwareType::Cpu:
					Console::WriteLine("CPU: " + hardware->Name);
					break;
				case HardwareType::GpuAmd:
				case HardwareType::GpuIntel:
				case HardwareType::GpuNvidia:
					Console::WriteLine("GPU: " + hardware->Name);
					break;
				default:
					Console::WriteLine("Error: Hardware type not recognized.");
					break;
				}

				for each (ISensor ^ sensor in hardware->Sensors) {
					if (sensor->SensorType == SensorType::Temperature && // Verifica se o sensor é do tipo temperatura e se tem um valor válido (maior que 0)
						sensor->Value.HasValue &&
						sensor->Value.Value > 0) {
						Console::WriteLine(sensor->Name + ": " + sensor->Value + " C");
					}
				}
			}
		}

		System::Threading::Thread::Sleep(1000); // Aguarda 1 segundo antes de atualizar os dados novamente
	}

    return 0;
}
