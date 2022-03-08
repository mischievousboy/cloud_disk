CREATE DATABASE `cloud_disk`;
USE cloud_disk;
CREATE TABLE `user_info`
(
    `id`          bigint(20)                        NOT NULL AUTO_INCREMENT COMMENT '自增主键',
    `uid`         varchar(10) CHARACTER SET utf8mb4 NOT NULL COMMENT '用户UID',
    `user_name`   varchar(30) CHARACTER SET utf8mb4 NOT NULL DEFAULT '' COMMENT '用户名',
    `nick_name`   varchar(30) CHARACTER SET utf8mb4 NOT NULL COMMENT '昵称',
    `password`    varchar(30) CHARACTER SET utf8mb4 NOT NULL COMMENT '密码',
    `phone`       varchar(30) CHARACTER SET utf8mb4 NOT NULL COMMENT '电话号码',
    `email`       varchar(30) CHARACTER SET utf8mb4 NOT NULL COMMENT '邮箱',
    `create_time` datetime                          NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `update_time` datetime                          NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`),
    UNIQUE KEY `user_name_UNIQUE` (`user_name`),
    UNIQUE KEY `uid_UNIQUE` (`uid`)
) ENGINE = InnoDB
  AUTO_INCREMENT = 1
  DEFAULT CHARSET = utf8mb4
  COLLATE = utf8mb4_unicode_ci
  ROW_FORMAT = DYNAMIC COMMENT ='用户基础信息表';

CREATE TABLE `user_file_info`
(
    `id`          bigint(20)                        NOT NULL AUTO_INCREMENT COMMENT '自增主键',
    `uid`         varchar(10) CHARACTER SET utf8mb4 NOT NULL COMMENT '用户UID',
    `file_name`   varchar(30) CHARACTER SET utf8mb4 NOT NULL COMMENT '用户填写的文件名',
    `file_url`    varchar(45) CHARACTER SET utf8mb4 NOT NULL COMMENT '文件存储的url',
    `create_time` datetime                          NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `update_time` datetime                          NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  AUTO_INCREMENT = 1
  DEFAULT CHARSET = utf8mb4
  COLLATE = utf8mb4_unicode_ci
  ROW_FORMAT = DYNAMIC COMMENT ='文件信息表';