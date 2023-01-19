#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "connection_pool.h"

void op1(int begin, int end) {
  for (int i = begin; i < end; i++) {
    MysqlConnection conn;
    conn.Connect("root", "127.0.0.1", "Sq17273747@", "server_db", 3306);
    std::string sql = "insert into user values('zhangsan', '123456')";
    conn.Update(sql);
  }
}

void op2(ConnectionPool* pool, int begin, int end) {
  for (int i = begin; i < end; i++) {
    std::shared_ptr<MysqlConnection> conn = pool->GetConnection();
    std::string sql = "insert into user values('zhangsan', '123456')";
    conn->Update(sql);
  }
}

void test1() {
#if 0 
  std::chrono::steady_clock::time_point begin =
      std::chrono::steady_clock::now();
  op1(0, 5000);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  auto interval = end - begin;
  // 单线程, 不使用数据库连接池用时: 13525404739纳秒, 13525毫秒
  std::cout << "单线程, 不使用数据库连接池用时: " << interval.count()
            << "纳秒, " << interval.count() / 1000000 << "毫秒" << std::endl;
#else
  ConnectionPool* pool = ConnectionPool::GetInstance();
  std::chrono::steady_clock::time_point begin =
      std::chrono::steady_clock::now();
  op2(pool, 0, 5000);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  auto interval = end - begin;
  std::cout << "单线程, 使用数据库连接池用时: " << interval.count() << "纳秒, "
            << interval.count() / 1000000 << "毫秒" << std::endl;
  // 单线程, 使用数据库连接池用时: 5822742775纳秒, 5822毫秒
#endif
}

int main() { test1(); }