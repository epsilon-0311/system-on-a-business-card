
#!/bin/bash

usage() { echo "Usage: $0 -p <project directory>  -s <serial interface>" 1>&2; exit 1; }

while getopts "p:s:j:" o; do
    case "${o}" in
		p)
			p=${OPTARG}
			;;
		s)
			s=${OPTARG}
			;;
		*)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if  [ -z "${p}" ]; then
    usage
fi

if  [ -v s ]; then
    serial="-v ${s}:/dev/ttyUSB0"
fi


# check if podman or docker is installed
if [ -x "$(command -v podman)" ]; then
    echo 'Found podman installation' >&2
    CMD=podman
elif [ -x "$(command -v docker)" ]; then
    echo 'Found docker installation' >&2
    CMD=docker
else 
    echo 'Found neither docker nor podman installation' >&2
    exit 1
fi

cnt=$(podman ps -a | grep iot-container | wc -l)
#  -v /dev/usb:/dev/usb -v /run/udev:/run/udev:ro
$CMD run --rm -it --name dev-env-container-${cnt} ${serial} ${jlink} \
	 --network host --privileged -v ${p}:/workingdir/project  --workdir /workingdir/project  --group-add keep-groups \
	ghcr.io/epsilon-0311/zephyr-dev-env:latest