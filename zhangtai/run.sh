#/bin/bash
old_lib_path=${LD_LIBRARY_PATH}
echo "old_lib_path:" ${old_lib_path}


cd lib
rm -f libpaddle_capi_shared.so
cd ..

cp build/exeModel ./exeModel
arr=("now" "oldlarge" "now_debug")
for path in "${arr[@]}"
do
        export LD_LIBRARY_PATH=${path}:${old_lib_path}
        echo "test time of ${LD_LIBRARY_PATH}"
        ./exeModel
done

cd lib
ln -s ../oldlarge/libpaddle_capi_shared.so .
cd ..
