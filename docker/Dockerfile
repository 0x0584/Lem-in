FROM python:latest

RUN apt update && apt upgrade -y
RUN apt install -y vim
RUN pip install plotly networkx kaleido matplotlib
RUN mkdir /workspace
WORKDIR /workspace

CMD ["bash"]
