CREATE DATABASE `cloud_disk`;
USE cloud_disk;
CREATE TABLE `user_info`
(
    `id`          bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增主键',
    `user_name`    varchar(30) CHARACTER SET utf8mb4 NOT NULL DEFAULT '' COMMENT '用户名',
    `nick_name`   varchar(30) CHARACTER SET utf8mb4 NOT NULL COMMENT '昵称',
    `password`   varchar(30) CHARACTER SET utf8mb4 NOT NULL COMMENT '密码',
    `phone`   varchar(30) CHARACTER SET utf8mb4 NOT NULL COMMENT '电话号码',
    `email`   varchar(30) CHARACTER SET utf8mb4 NOT NULL COMMENT '邮箱',
    `create_time` datetime                          NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `update_time` datetime                          NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`),
    UNIQUE KEY `user_name_UNIQUE` (`user_name`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC COMMENT='用户基础信息表';
