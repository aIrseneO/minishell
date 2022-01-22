FROM ubuntu:18.04

RUN apt-get update && apt-get install -y gcc make git

CMD ["bash"]

#docker build -t cbuilder
#docker run -it --rm -v $(pwd):/app cbuilder bash -c "make  -C /app test"
