#include "mysql_connection.h"

/**
 * @brief 构造MysqlConnection对象
 * @details 使用mysql_init()对连接句柄进行初始化
 */
MysqlConnection::MysqlConnection()
    : connection_(nullptr), result_(nullptr), row_(nullptr) {
  connection_ = mysql_init(nullptr);

  if (!connection_) {
    throw std::runtime_error("connection initialize error");
  }

  mysql_set_character_set(connection_, "utf8");
}

MysqlConnection::~MysqlConnection() {
  mysql_close(connection_);
  FreeResult();
}

/**
 * @brief 连接数据库
 *
 * @param user
 * @param ip
 * @param passwd
 * @param db
 * @param port
 * @return true  连接成功
 * @return false
 */
bool MysqlConnection::Connect(const std::string& user, const std::string& ip,
                              const std::string& passwd, const std::string& db,
                              unsigned int port) {
  MYSQL* ptr = mysql_real_connect(connection_, ip.c_str(), user.c_str(),
                                  passwd.c_str(), db.c_str(), port, nullptr, 0);

  return ptr != nullptr;
}

/**
 * @brief 执行更新语句
 *
 * @param sql  sql语句
 * @return true  更新成功
 * @return false
 */
bool MysqlConnection::Update(const std::string& sql) {
  // mysql_query() 成功时返回0
  if (mysql_query(connection_, sql.c_str())) {
    return false;
  }
  return true;
}

/**
 * @brief 执行查询语句, 并且将结果集合保存
 *
 * @param sql sql语句
 * @return true  查询成功
 * @return false
 */
bool MysqlConnection::Query(const std::string& sql) {
  if (mysql_query(connection_, sql.c_str())) {
    return false;
  }

  FreeResult();
  result_ = mysql_store_result(connection_);

  return result_ != nullptr;
}

/**
 * @brief 每调用一次Next(), 就可以获取到当前结果集合的下一行记录
 *
 * @return true
 * @return false  若当前已经是最后一行记录, 则失败
 */
bool MysqlConnection::Next() {
  if (result_) {
    // MYSQL_ROW的类型是 char**
    row_ = mysql_fetch_row(result_);
    return row_ != nullptr;
  }

  return false;
}

/**
 * @brief 获取到当前遍历的行中, 指定列的字段
 * @param index 列的下标
 * @return string 字段的值
 */
std::string MysqlConnection::value(int index) {
  int col_cnt = mysql_num_fields(result_);
  if (index < 0 || index >= col_cnt) {
    return "";
  }

  unsigned long length = mysql_fetch_lengths(result_)[index];
  char* val = row_[index];

  // 为了避免字段中包含\0, 必须手动获得当前字段长度
  return std::string(val, length);
}

/**
 * @brief 关闭自动commit模式
 *
 * @return true
 * @return false
 */
bool MysqlConnection::DisableAutoCommitMode() {
  return mysql_autocommit(connection_, false);
}

/**
 * @brief 提交事务
 *
 * @return true
 * @return false
 */
bool MysqlConnection::Commit() { return mysql_commit(connection_); }

/**
 * @brief 事务回滚
 *
 * @return true
 * @return false
 */
bool MysqlConnection::Rollback() { return mysql_rollback(connection_); }

/**
 * @brief 释放result_所指向的内存
 *
 */
void MysqlConnection::FreeResult() {
  if (result_) {
    mysql_free_result(result_);
  }
}

/**
 * @brief 刷新当前连接的入池时间点
 *
 */
void MysqlConnection::RefreshTime() {
  time_point_ = std::chrono::steady_clock::now();
}

/**
 * @brief 返回当前连接在连接池中的空闲时间
 *
 * @return long long
 */
long long MysqlConnection::GetAliveTime() {
  auto nano_interval = std::chrono::steady_clock::now() - time_point_;
  auto mili_interval =
      std::chrono::duration_cast<std::chrono::milliseconds>(nano_interval);

  return mili_interval.count();
}