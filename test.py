import subprocess
import os

# Указание полного пути к директории сборки
build_dir = r"C://Users//zubov//Desktop//ServerMessengerProj//build-client"

# Проверяем, существует ли указанный путь
if not os.path.exists(build_dir):
    print(f"Directory {build_dir} does not exist.")
else:
    # Параметры для открытия нового окна консоли
    creationflags = subprocess.CREATE_NEW_CONSOLE

    # Запуск первого экземпляра
    subprocess.Popen([os.path.join(build_dir, 'client.exe'), '127.0.0.1', '12345'], creationflags=creationflags)

    # Запуск второго экземпляра
    subprocess.Popen([os.path.join(build_dir, 'client.exe'), '127.0.0.1', '12345'], creationflags=creationflags)

    # Добавьте дополнительные экземпляры, если нужно
    subprocess.Popen([os.path.join(build_dir, 'client.exe'), '127.0.0.1', '12345'], creationflags=creationflags)

    print("Запущены все экземпляры клиента.")
