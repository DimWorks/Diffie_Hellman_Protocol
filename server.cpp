#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "WS2_32.lib") // указывает компоновщику, что требуется файл Ws2_32.lib

#pragma warning(disable: 4996)		// чтоб кмпилятор не ругался на inet_addr("127.0.0.1")

using namespace std;

int mod(int a, int x, int p)
{
	int res = 1;
	for (int i = 1; i <= x; i++)
	{
		res = (res * a) % p;
	}
	return res;
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

	SOCKET sockListener = socket(AF_INET, SOCK_STREAM, NULL);

	bind(sockListener, (SOCKADDR*)&addr, sizeof(addr));
	listen(sockListener, SOMAXCONN/*Максимальное число соединений*/);

	cout << "Ошидание подключения клиента...\n";

	SOCKET newConnect;
	int sizeofAddr = sizeof(addr);
	newConnect = accept(sockListener, (SOCKADDR*)&addr, &sizeofAddr); // Содержит сведениья о клиенте

	if (newConnect == 0)
	{
		cout << "ERROR: Клиент не смог подключиться" << endl;
	}
	else
	{
		cout << "Complite: Соединение с клиентом установлено" << endl;

		int publicKeyA;
		int secretKeyB, publicKeyB;

		int p = 23; // Задаем общее простое число p
		int base = 25;  // Задаем общий первообразный корень g

		// Генерируем секретный ключ и публичный ключ для сервера
		srand(time(NULL));
		secretKeyB = rand() % p;
		if (secretKeyB < 0)
		{
			secretKeyB *= -1;
		}
		publicKeyB = mod(base, secretKeyB, p);

		// Отправляем публичный ключ клиенту
		send(newConnect, (char*)&publicKeyB, sizeof(publicKeyB), 0);

		// Получаем публичный ключ от клиента
		recv(newConnect, (char*)&publicKeyA, sizeof(publicKeyA), 0);

		// Вычисляем общий секретный ключ
		int sharedSecretKey = mod(publicKeyA, secretKeyB, p);

		std::cout << "Общий секретный ключ: " << sharedSecretKey << std::endl;
	}

	return 0;
}