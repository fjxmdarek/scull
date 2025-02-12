#ifndef SCULL_HELPERS_H
#define SCULL_HELPERS_H

struct scull_dev; // 前向声明

/* 用于遍历或扩展链表，返回第 n 个节点 */
struct scull_qset *scull_follow(struct scull_dev *dev, int n);

/* 清空并释放整个 scull 设备中的链表 */
int scull_trim(struct scull_dev *dev);

#endif /* SCULL_HELPERS_H */