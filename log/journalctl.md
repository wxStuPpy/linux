# 查看所有日志（分页显示，支持搜索，按 '/' 输入关键词）
sudo journalctl

# 查看当前开机周期的日志
sudo journalctl -b

# 按时间过滤（例如：最近 1 小时）
sudo journalctl --since "1 hour ago"

# 查看特定服务的日志（如 sshd 服务）
sudo journalctl -u sshd.service

# 实时监控日志（类似 tail -f）
sudo journalctl -f