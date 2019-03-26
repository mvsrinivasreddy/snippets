wan=`ifconfig | grep -A 1 enp3s0 | awk -v FS="(Bcast:|Mask
)" '{print $2}'`
echo $wan

wanip=${wan%% Mask*}
echo $wanip

finalwan=`echo $wanip | cut -d"." -f1-3`
#finalwan=${wanip%% .255*}
#finalwan="${var%.*}.0"
echo $finalwan

for i in $(seq 255);
do
 ping -W 1 -c 1 $finalwan.$i | grep -q 'from' &
done

