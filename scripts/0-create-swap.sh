cd ~
sudo fallocate -l 2G swapfile
sudo chmod 600 swapfile 
sudo mkswap swapfile 
sudo swapon swapfile
sudo fallocate -l 4G swapfile_BIG
sudo chmod 600 swapfile 
sudo mkswap swapfile 
sudo swapon swapfile
sudo fallocate -l 16G swapfile_BIGGER
sudo chmod 600 swapfile 
sudo mkswap swapfile 
sudo swapon swapfile
sudo swapon --show
