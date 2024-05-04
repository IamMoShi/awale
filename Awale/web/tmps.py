import subprocess
import json

p1 = subprocess.run(["./CAwale.exe", "4", "0", "4", "4", "4", "4", "4", "4", "4", "4", "4", "4", "0", "0", "100000"],
                    capture_output=True)

# test_string = '{"Nikhil" :\n {"a" : 1}, "Akshat" : 2, "Akash" : 3,}'
# res = json.loads(test_string)
# print(res.get("Nikhil"))


data = json.loads(p1.stdout.decode())
print(data)