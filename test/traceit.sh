function traceit(){
    local location=${1:?error please specify a name}
    local name=${2:?error please specify a name}
    local of=${location}/boost_unlink/$(hostname)/${name}.log

    shift
    shift
    
    mkdir -pv $(dirname $of)
    strace -Ff -tt  -T $@ > ${of} 2>&1 
}

function test-same-dir-unlink(){
  local location=${1}
  if  [[ $location =~ ^--.* ]] || [[ $location == "" ]] ; then
      location=/tmp/strace
  else
      shift
  fi

  traceit $location "001/file" boost_unlink testing --files 1    "$@"
  traceit $location "010/file" boost_unlink testing --files 10   "$@"
  traceit $location "100/file" boost_unlink testing --files 100  "$@"
}

function test-directoryperdb-unlink(){
  local location=${1}
  if  [[ $location =~ ^--.* ]] || [[ $location == "" ]] ; then
      location=/tmp/strace
  else
      shift
  fi

  traceit $location "001/dpdb" boost_unlink testing --files 1   --directoryperdb  "$@"
  traceit $location "010/dpdb" boost_unlink testing --files 10  --directoryperdb  "$@"
  traceit $location "100/dpdb" boost_unlink testing --files 100 --directoryperdb  "$@"
}
