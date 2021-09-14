docker-compose down
brew install kafka
docker-compose up -d
sleep 10
kafka-topics --create --zookeeper localhost:22181 --replication-factor 1 --partitions 1 --topic test