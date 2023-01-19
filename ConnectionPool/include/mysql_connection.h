#ifndef MYSQL_CONNECTION_H
#define MYSQL_CONNECTION_H

#include <chrono>
#include <stdexcept>
#include <string>

#include "mysql/mysql.h"

class MysqlConnection {
 public:
  MysqlConnection();

  ~MysqlConnection();

  bool Connect(const std::string& user, const std::string& ip,
               const std::string& passwd, const std::string& db,
               unsigned int port);

  bool Update(const std::string& sql);

  bool Query(const std::string& sql);

  bool Next();

  std::string value(int index);

  bool DisableAutoCommitMode();

  bool Commit();

  bool Rollback();

  void RefreshTime();

  long long GetAliveTime();

 private:
  void FreeResult();

 private:
  // connection_在析构函数中释放
  // result_存储的是返回结果, 在每次查询之前, 都应该先把result_指向的内存释放,
  // 此外, 在析构时, result_指向的依旧可能是一块有效内存 所以,
  // 析构时也应该进行释放操作 row_所指向的结构是result_所指向结构的一部分,
  // 不需要单独释放
  MYSQL* connection_;  // 当前连接
  MYSQL_RES* result_;  // 查询结果
  MYSQL_ROW row_;      // 当前遍历的行

  // 当前连接进入连接池的时间点
  // 每次重新进入连接池的时候都会被刷新
  std::chrono::steady_clock::time_point time_point_;
};
#endif