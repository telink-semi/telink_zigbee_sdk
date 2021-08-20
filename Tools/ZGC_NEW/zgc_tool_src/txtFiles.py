import os
from datetime import datetime


class TxtFiles:
    def __init__(self):
        self.create_folder_path = ''
        self.all_packets_path = ''
        self.all_packets_file_name = '/thread_packets.txt'

    def create_folder(self):
        nowtime_strfile = datetime.now().strftime('%y-%m-%d-%H-%M-%S')
        is_exit = os.path.exists('./userdata')
        if not is_exit:
            os.mkdir('./' + 'userdata')
        self.create_folder_path = './userdata' + '/' + nowtime_strfile + 'thread'
        os.mkdir(self.create_folder_path)  # 创建文件夹
        self.all_packets_path = self.create_folder_path + self.all_packets_file_name  # 创建一个all_packets的文件
        return self.create_folder_path

    def write_file(self, time_insert, datastr, direct):
        write_result = 1
        if not os.path.exists(self.all_packets_path):  # 防止文件或文件夹被误删
            self.create_folder()
        try:
            with open(self.all_packets_path, 'a', encoding='utf-8') as f:
                f.write('\n' + time_insert + ' ' + direct + ':\n')
                f.write(datastr)
        except FileNotFoundError:
            self.create_folder()
        except PermissionError:
            write_result = 3
        return write_result
