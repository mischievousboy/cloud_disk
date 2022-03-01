构建编译镜像，第三方库git拉取，需设置git代理<br>
``docker build  -t build-os:latest -f ./ci/Dockerfile-build-os --network host --build-arg HTTP_PROXY=http://xx.xx.xx.xx:1080  . ``<br>
编译镜像<br>
``docker-compose -p cloud-disk build`` <br>
应用启动，注意docker-compose启动因mysql初始化比较慢，服务启动无法连接数据库，需在mysql完全启动之后再重启服务，推荐使用stack启动避免这个问题<br>
    1.``docker-compose -p cloud-disk up`` <br>2.``docker stack deploy -c docker-stack.yaml cloud-disk``