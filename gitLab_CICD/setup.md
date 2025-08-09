1、使用dokcer拉取gitlab CE镜像并启动
sudo docker run --detach \      #后台运行docker
  --hostname gitlab.local \     #设置容器内部的主机名(GitLab默认使用这个域名生成链接)
  --publish 8443:443 --publish 8080:80 --publish 2222:22 \ #将容器的HTTPS(443)端口映射到宿主机的8443端口 将容器的HTT(80)端口映射到宿主机的8080端口,将容器的SSH(22)端口映射到宿主机的2222端口
  --name gitlab \                                           #将这个容器起名为gitlab
  --restart always \                                        #容器异常时自动重启(防止崩溃)
  --volume $HOME/gitlab/config:/etc/gitlab \                #gitlab配置目录永久化
  --volume $HOME/gitlab/logs:/var/log/gitlab \              #日志目录映射到宿主机，方便查看日志
  --volume $HOME/gitlab/data:/var/opt/gitlab \              #主要数据目录(仓库数据，数据库等)持久化
  gitlab/gitlab-ce:latest                                   #使用GitLab CE(社区版)的最新镜像

  运行后可以通过htts://localhost:8080地址访问
  初始密码:sudo cat $HOME/gitlab/config/initial_root_password


2、安装并注册gitlab runner程序
# 官方安装脚本
curl -L https://packages.gitlab.com/install/repositories/runner/gitlab-runner/script.deb.sh | sudo bash
sudo apt install -y gitlab-runner

注册Runner
sudo gitlab-runner register
按提示填写：

GitLab URL：http://localhost:8080

Token：在 GitLab 项目页面 → Settings → CI/CD → Runners → 获取 Registration Token

描述：local-runner

Tags：local

Executor：选 shell
3、创建项目
mkdir visual_tool && cd visual_tool
mkdir backend cppcore frontend
touch .gitlab-ci.yml README.md
echo -e "print('hello world')" > backend/main.py
git init
git remote add origin http://localhost:8080/root/visual_tool.git  # 改成你的地址
git add .
git commit -m "init"
git push -u origin main

4、写.gitlab-ci.yml
stages:               #定义流水线的阶段流程程序，在GitLab CI/CD中，流水线被划分为多个阶段，阶段按顺序执行，阶段内的job可以并行执行
  - test              #这个.yml文件暂时只有1个test阶段

test_backend:        #具体的Job名，GitLab页面会显示这个名称。它描述了test阶段应该执行那些命令
  stage: test       #把当前job分配到test阶段，与上面的stages对应
  script:           #该Job执行的具体命令列表。每条命令相当于在shell里逐条执行
    - echo "Running Python backend"
    - python3 backend/main.py
  tags:             #这个字段告诉GitLab：这个Job应该被那些Runner执行,如果不加tags,GitLab就会提示找不到合适的Runner来执行这个任务
    - local

