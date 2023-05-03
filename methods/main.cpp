/**
 * @file methods/main.cpp
 * @author Mikhail Lozhnikov
 *
 * Файл с функией main() для серверной части программы.
 */

#include "methods.hpp"
#include <httplib.h>
#include <iostream>
#include <nlohmann/json.hpp>

int main(int argc, char *argv[]) {
  // Порт по-умолчанию.
  int port = 8080;

  if (argc >= 2) {
    // Меняем порт по умолчанию, если предоставлен соответствующий
    // аргумент командной строки.
    if (std::sscanf(argv[1], "%d", &port) != 1)
      return -1;
  }

  std::cerr << "Listening on port " << port << "..." << std::endl;

  httplib::Server svr;

  // Обработчик для GET запроса по адресу /stop. Этот обработчик
  // останавливает сервер.
  svr.Get("/stop", [&](const httplib::Request&, httplib::Response&) { 
    svr.stop();
  });

  /* Сюда нужно вставить обработчик post запроса для алгоритма. */

  svr.Post("/bridge_search", [&](const httplib::Request &req, httplib::Response &res) {
    const nlohmann::json js = nlohmann::json::parse(req.body);
    nlohmann::json *result = new nlohmann::json();
    graph::BridgeSearchMethod(js, result);
    res.set_content((*result).dump(), "application/json");
  });

  /* Конец вставки. */

  // Эта функция запускает сервер на указанном порту. Программа не завершится
  // до тех пор, пока сервер не будет остановлен.
  svr.listen("0.0.0.0", port);

  return 0;
}