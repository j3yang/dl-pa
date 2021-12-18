import sys
import subprocess
import os

tests_dir = os.path.abspath(os.path.dirname(__file__))
dlpa_path = tests_dir + '/../build/dlpa'

def run_tests_in_subdir(subdir):
    test_subdir = tests_dir + '/' + subdir
    in_subdir = test_subdir + '/in'
    out_subdir = test_subdir + '/out'
    if not os.path.exists(out_subdir):
        os.makedirs(out_subdir)
    eout_subdir = test_subdir + '/expected_out'
    in_files_list = os.listdir(in_subdir)
    in_files_list.sort()
    for in_file in in_files_list:
        in_file_path = in_subdir + '/' + in_file
        out_file_path = out_subdir + '/' + in_file[:-2] + 'out'
        subprocess.run([dlpa_path, "-c", "-f", in_file_path, "-o", out_file_path])
    out_files_list = os.listdir(out_subdir)
    out_files_list.sort()
    eout_files_list = os.listdir(eout_subdir)
    eout_files_list.sort()
    for out_file, eout_file in zip(out_files_list, eout_files_list):
        out_file_path = out_subdir + '/' + out_file
        eout_file_path = eout_subdir + '/' + eout_file
        run_result = subprocess.run(['diff', out_file_path, eout_file_path], capture_output=True).stdout.decode("utf-8")
        if run_result:
            print(f"Test {out_file[:-4]} FAILED")
            print(run_result)
        else:
            print(f"Test {out_file[:-4]} PASSED")

def run_tests_in_all_subdirs():
    for subdir in os.listdir(tests_dir):
        if os.path.isdir(tests_dir + '/' + subdir):
            run_tests_in_subdir(subdir)

if __name__ == "__main__":
    # top < atom < neg < or < assign < test < seq < cup < star in terms of dependencies within test
    if len(sys.argv) > 1:
        for subdir in sys.argv[1:]:
            run_tests_in_subdir(subdir)
    else:
        run_tests_in_all_subdirs()
    