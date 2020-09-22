FROM ubuntu
COPY . /ilja_c
WORKDIR /ilja_c
#RUN gcc -Og -pg ch5_517.c -o prog
#CMD ["./prog"]
#CMD ["gprof prog"]
ENTRYPOINT [ "/bin/bash", "-c", "--" ]
CMD [ "while true; do sleep 30; done;" ]