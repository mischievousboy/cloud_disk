构建编译镜像，第三方库git拉取，需设置git代理<br>
``docker build  -t build-os:latest -f ./ci/Dockerfile-cloud-disk --network host --build-arg HTTP_PROXY=http://xx.xx.xx.xx:1080  . ``<br>
应用启动<br>
``docker-compose up``