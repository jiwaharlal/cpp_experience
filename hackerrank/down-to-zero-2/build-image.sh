dockerImageTag="debian-build-shell"

docker build . -f Dockerfile -t $dockerImageTag
