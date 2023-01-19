#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <memory>
#include <thread>

#include "mysql_connection.h"
#include "nlohmann/json.hpp"

class ConnectionPool {
 public:
  static ConnectionPool* GetInstance();

  ConnectionPool(const ConnectionPool&) = delete;
  ConnectionPool& operator=(const ConnectionPool&) = delete;

  std::shared_ptr<MysqlConnection> GetConnection();
  ~ConnectionPool();

 private:
  using json = nlohmann::json;

  ConnectionPool();
  void ParseJson();

  void ProduceConnection();
  void DestroyConnection();

  void AddConnection();


 private:
  std::string user_;    // 用户名
  std::string passwd_;  // 密码
  std::string ip_;      // 数据库ip地址
  unsigned int port_;   // 端口号
  std::string db_;      // 数据库名称
  int min_size_;        // 连接池中的最小连接数
  int max_size_;        // 连接池中的最大连接数目
  int max_idle_time_;   // 空闲连接的最大存活时间
  int timeout_;         // 线程等待连接的最大超时事件

  std::queue<MysqlConnection*> connections_;
  std::condition_variable condition_;
  std::mutex mutex_;
};
#endif