output_path=/app/inf/mpi/bml-guest/paddlepaddle/gongweibao/transformer/en-fr/output 
${HADOOP_BIN} fs -Dhadoop.job.ugi=${HADOOP_UGI} -fs ${HADOOP_FS} -rmdir ${output_path}
