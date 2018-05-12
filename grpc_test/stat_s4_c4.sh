arr=(4 16 32 64 128 256 512 1024 2048)
for i in "${arr[@]}"
do
 size=$((i*1024))
 max=`cat *_$size.log | grep begin| awk -F "[:, ]" '{print $9}' | awk  'BEGIN{max=0} {if($1>max+0) max=$1} END{print max}'`

 echo $size, $max
 echo $size*1000.0*4.0*512.0/1024.0/1024.0/$max | bc
done
