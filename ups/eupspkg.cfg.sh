# EupsPkg config file. Sourced by 'eupspkg'

config() { :; }
build() { :; }
install() 
{
    die_if_empty PRODUCT
    die_if_empty VERSION

    clean_old_install
    mkdir -p "${PREFIX}"
    cp -r `ls | egrep -v '^ups'` ${PREFIX}/.
    install_ups
}


