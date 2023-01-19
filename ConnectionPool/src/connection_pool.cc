#include "connection_pool.h"

/**
 * @brief 获取全局静态实例
 *
 * @return ConnectionPool*
 */
ConnectionPool* ConnectionPool::GetInstance() {
  static ConnectionPool pool;
  return &pool;
}

/**
 * @brief 构造函数
 *
 */
ConnectionPool::ConnectionPool() : stop_(false) {
  ParseJson();

  for (int i = 0; i < min_size_; i++) {
    AddConnection();
  }

  std::thread producer([this] { ProduceConnection(); });
  std::thread consumer([this] { DestroyConnection(); });

  producer.detach();
  consumer.detach();
}

ConnectionPool::~ConnectionPool() {
  std::lock_guard<std::mutex> locker(mutex_);
  while (!connections_.empty()) {
    auto connection = connections_.front();
    connections_.pop();
    delete connection;
  }
  // 让正处于wait状态的消费者子线程终止
  stop_ = true;
  condition_.notify_all();
}

/**
 * @brief 方便地从数据库连接池中获取一个连接
 *         使用了智能指针, 用户不需要手动将其加入连接池
 *
 * @return std::shared_ptr<MysqlConnection>
 */
std::shared_ptr<MysqlConnection> ConnectionPool::GetConnection() {
  std::unique_lock<std::mutex> locker(mutex_);

  // 当从wait_for中返回时, 线程可能是超时, 也可能是被唤醒
  while (connections_.empty()) {
    if (std::cv_status::timeout ==
        condition_.wait_for(locker, std::chrono::milliseconds(timeout_))) {
      // if (connections_.empty()) {
      //   // return nullptr;
      //   continue;
      // }
      continue;
    }
  }

  // 当线程使用完数据库连接之后, 应该将连接归还到数据库连接池中
  auto destructor = [this](MysqlConnection* conn) {
    std::lock_guard<std::mutex> locker(mutex_);
    conn->RefreshTime();
    connections_.push(conn);
  };

  std::shared_ptr<MysqlConnection> connection(connections_.front(), destructor);
  connections_.pop();
  condition_.notify_all();  // 用于通知生产者线程

  return connection;
}
/**
 * @brief 解析json配置文件
 *
 */
void ConnectionPool::ParseJson() {
  std::ifstream input("dbconfig.json");
  json data = json::parse(input);

  if (!data.is_object()) {
    throw std::runtime_error("json config error");
  }

  user_ = data["user"];
  passwd_ = data["passwd"];
  ip_ = data["ip"];
  port_ = data["port"];
  db_ = data["db"];
  min_size_ = data["min_size"];
  max_size_ = data["max_size"];
  max_idle_time_ = data["max_idle_time"];
  timeout_ = data["timeout"];
}

/**
 * @brief 生产者线程
 *
 */
void ConnectionPool::ProduceConnection() {
  while (true) {
    std::unique_lock<std::mutex> locker(mutex_);
    while (connections_.size() >= min_size_ && !stop_) {
      condition_.wait(locker);
    }

    AddConnection();
    condition_.notify_all();  // 唤醒所有消费者
  }
}

/**
 * @brief 清理线程
 *
 */
void ConnectionPool::DestroyConnection() {
  while (!stop_) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::lock_guard<std::mutex> locker(mutex_);

    while (connections_.size() > min_size_) {
      auto connection = connections_.front();
      // 线程池中线程的数量过多是需要清理连接
      // 队列首部元素的空闲时间肯定是最长的, 所以从头部开始清理
      if (connections_.size() > max_size_ ||
          connection->GetAliveTime() >= max_idle_time_) {
        connections_.pop();
        delete connection;
      } else {
        break;
      }
    }
  }
}
/**
 * @brief 添加一个新的Mysql连接到连接池当中
 *
 */
void ConnectionPool::AddConnection() {
  MysqlConnection* conn = new MysqlConnection;
  bool ret = false;

  ret = conn->Connect(user_, ip_, passwd_, db_, port_);
  if (!ret) {
    throw std::runtime_error("connection initialize error");
  }
  conn->RefreshTime();
  connections_.push(conn);
}
