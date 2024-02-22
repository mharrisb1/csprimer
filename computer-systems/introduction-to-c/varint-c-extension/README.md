# Varint C Extension

## Build

```sh
python -m venv .venv                                            # create venv
source .venv/bin/activate                                       # activate venv
pip install build setuptools                                    # install build deps
python -m build                                                 # build extension
pip install ./dist/cvarint-1.0.0-cp312-cp312-linux_aarch64.whl  # install extension
```
