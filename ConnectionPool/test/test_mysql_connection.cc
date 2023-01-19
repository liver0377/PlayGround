#include <cassert>
#include <iostream>
#include <string>

#include "mysql_connection.h"

int main() {
  MysqlConnection conn;
  bool ret = false;

  ret = conn.Connect("root", "127.0.0.1", "Sq17273747@", "server_db", 3306);
  assert(ret == true);

  std::string sql = "insert into user values('zhangsan', '123456')";
  ret = conn.Update(sql);
  assert(ret == true);

  sql = "select * from user";
  ret = conn.Query(sql);
  assert(ret == true);

  while (conn.Next()) {
    std::string name = conn.value(0);
    std::string passwd = conn.value(1);

    std::cout << "name: " << name << "passwd: " << passwd << std::endl;
  }

  return 0;
}