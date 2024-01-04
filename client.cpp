#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "WS2_32.lib") // указывает компоновщику, что требуется файл Ws2_32.lib

#pragma warning(disable: 4996)

using namespace std;

int power(int a, int b, int P)
{
	if (b == 1)
		return a;
	else
		return int(fmod(pow(a, b), P));
}

int main()
{
	setlocale(LC_ALL, "RU");
	//WSAStartup

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1); // Запрашиваемая версия библиотеки

	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		// Если библиотека не загрузилась
		cout << "ERROR: Библиотека WAS не запустилась" << endl;
		exit(1);
	}

	// Заполняем информацию о сокете

	SOCKADDR_IN addr;

	addr.sin_port = htons(5555);						// Порт, на котором работает сервер
	addr.sin_family = AF_INET;							// Семейство протоколов, для интеренет протоколов указывается константа AF_INET
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");		// IP-адрес сервера


	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, NULL);


	if (connect(sockClient, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		cout << "ERROR: Не смог подключиться к серверу" << endl;
		exit(1);
	}
	else
	{
		cout << "Complite: Соединение с сервером установлено" << endl;

		int p = 23; // Общее простое число p
		int g = 25;  // Общий первообразный корень g

		int secretKeyA, publicKeyA;
		int secretKeyB, publicKeyB;

		// Генерируем секретный ключ и публичный ключ для клиента
		srand(time(NULL));
		secretKeyA = rand() % p;
		if (secretKeyA < 0)
		{
			secretKeyA *= -1;
		}
		publicKeyA = int(fmod(pow(g, secretKeyA), p));

		// Отправляем публичный ключ серверу
		send(sockClient, (char*)&publicKeyA, sizeof(publicKeyA), 0);

		// Получаем публичный ключ от сервера
		recv(sockClient, (char*)&publicKeyB, sizeof(publicKeyB), 0);

		// Вычисляем общий секретный ключ
		int sharedSecretKey = power(publicKeyB, secretKeyA, p);

		std::cout << "Общий секретный ключ: " << sharedSecretKey << std::endl;
	}



	return 0;
}