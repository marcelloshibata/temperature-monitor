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

	int top = Console::CursorTop; // Armazena a posição inicial do cursor para evitar que a tela fique piscando
	while (true) {
		Console::SetCursorPosition(0, top); // Move o cursor de volta para a posição inicial a cada atualização para evitar que a tela fique piscando

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
						if (sensor->SensorType == SensorType::Temperature && sensor->Value.HasValue && sensor->Value.Value > 0) { // Verifica se o sensor é do tipo temperatura, se tem um valor válido e se o valor é maior que 0 para evitar mostrar temperaturas negativas ou sensores sem leitura.
								float temperatureDecimal = sensor->Value.Value; // Pega o valor do sensor de temperatura e transforma em Float para formatar apenas com duas casas decimais.
								Console::WriteLine(sensor->Name + ": " + temperatureDecimal.ToString("F2") + " C");
						}
					}

			}
		}

		System::Threading::Thread::Sleep(1000); // Aguarda 1 segundo antes de atualizar os dados novamente
	}

    return 0;
}
