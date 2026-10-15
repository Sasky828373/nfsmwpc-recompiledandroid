#!/usr/bin/env python3
"""Prepare the locally owned Most Wanted server.dll for Android LAN hosting.

Generated analysis files and translated C stay ignored. The original DLL is
read only; supply the same DLL in the phone's nfsmw game-data directory.
"""
import argparse
import pathlib
import subprocess
import sys


def main():
    root = pathlib.Path(__file__).resolve().parent.parent
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("dll", type=pathlib.Path, help="server.dll from your PC copy")
    args = parser.parse_args()
    source = args.dll.resolve(strict=True)
    work = root / "work" / "server"
    work.mkdir(parents=True, exist_ok=True)
    image, seeds, catalog = (work / name for name in
                             ("server-relocated.dll", "seeds.json", "catalog.json"))
    steps = [
        ["prepare_module.py", source, "0x00e00000", image, seeds],
        ["run_lift.py", "catalog", image, seeds, catalog, "--seeds-only"],
        ["run_lift.py", "lift", image, catalog, root / "src/recomp/gen_server",
         "--prefix", "server_"],
    ]
    for script, *arguments in steps:
        subprocess.run([sys.executable, str(root / "scripts" / script),
                        *(str(arg) for arg in arguments)], cwd=root, check=True)


if __name__ == "__main__":
    main()
