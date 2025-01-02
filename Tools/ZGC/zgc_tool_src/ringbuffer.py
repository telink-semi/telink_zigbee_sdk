# import time
from settings import Settings


class RingBuffer:
    def __init__(self):
        self.ai_setting = Settings()
        self.recv_ring_buff = [0] * self.ai_setting.ring_buff_length
        self.recv_ring_length = self.ai_setting.ring_buff_length
        self.valid_data_length = 0
        self.write_ptr = 0
        self.read_ptr = 0

    def ring_buffer_write(self, str_list):
        write_success_flag = True
        if self.valid_data_length + len(str_list) > self.recv_ring_length:
            write_success_flag = False
            # print('没空间了！，写入：%d' % len(str_list))
            return write_success_flag

        # print('self.write_ptr:%d,len:%d' % (self.write_ptr, len(str_list)))
        for data in str_list:
            self.recv_ring_buff[self.write_ptr] = data
            self.valid_data_length += 1

            if self.write_ptr >= self.recv_ring_length - 1:
                self.write_ptr = 0
            else:
                self.write_ptr += 1
        # print('valid_data_length:%d' % self.valid_data_length)
        # print(self.recv_ring_buff)

        return write_success_flag

    def ring_buffer_data_get(self, length, get_str):
        while length > self.valid_data_length:
            return 0
        # print('self.read_ptr:%d' % self.read_ptr)
        for a in range(length):
            get_str[a] = self.recv_ring_buff[self.read_ptr]
            if self.read_ptr >= self.recv_ring_length - 1:
                self.read_ptr = 0
            else:
                self.read_ptr += 1

            self.valid_data_length -= 1
        return length

    def ring_buffer_get_packet(self, packet_buffer):
        get_result = False
        get_start = False
        packet_length = 0
        read_ptr_start = 0
        valid_len_start = 0

        if self.valid_data_length < self.ai_setting.command_length_min:
            return get_result, packet_length

        get_buffer = [0] * self.ai_setting.command_length_max
        while get_start is False and self.valid_data_length:
            read_ptr_start = self.read_ptr
            valid_len_start = self.valid_data_length
            if self.ring_buffer_data_get(1, get_buffer) == 1:
                if get_buffer[0] == self.ai_setting.command_start:
                    packet_buffer.append(get_buffer[0])
                    packet_length += 1
                    get_start = True
        '''
        get the command header
        Start 	Command identifier	  Length	Check Sum	  Payload	   End
        1Bytes	      2Bytes	      2 Bytes	1 Byte	
        '''
        if get_start:
            if self.ring_buffer_data_get(5, get_buffer) == 5:
                # print('ring_buffer_data_get,5')
                # print(get_buffer)
                for a in range(5):
                    packet_buffer.append(get_buffer[a])
                    packet_length += 1
                payload_length = packet_buffer[self.ai_setting.packet_length_idx_low]

                # print('payload_length:%d,valid data len:%d' % (payload_length, self.valid_data_length))
                if payload_length + 7 <= self.ai_setting.command_length_max:  # 总包长不应大于self.command_length_max
                    if self.ring_buffer_data_get(payload_length + 1, get_buffer) == payload_length + 1:
                        # print('ring_buffer_data_get,%d' % (payload_length + 1))
                        # print(get_buffer)
                        if get_buffer[payload_length] == self.ai_setting.command_end:
                            for a in range(payload_length + 1):
                                packet_buffer.append(get_buffer[a])
                                packet_length += 1
                            get_result = True
                        else:
                            packet_length = 0
                    else: #wait for the other data
                        # print('read all data fail')
                        packet_length = 0
                        self.read_ptr = read_ptr_start
                        self.valid_data_length = valid_len_start  # 恢复指针和长度
                else: #clear invalid length data
                    # print('not recv all data.')
                    packet_length = 0
            else: #wait for the other data
                # print('read head fail')
                packet_length = 0
                self.read_ptr = read_ptr_start
                self.valid_data_length = valid_len_start  # 恢复指针和长度
        # print(packet_buffer)
        return get_result, packet_length

    def ring_buffer_clear_all_data(self):
        self.read_ptr = 0
        self.write_ptr = 0
        self.valid_data_length = 0
