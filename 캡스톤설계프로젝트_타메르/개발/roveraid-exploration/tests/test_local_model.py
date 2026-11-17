from pathlib import Path
import tempfile
import unittest
from unittest.mock import Mock, patch

from roveraid.local_model import LocalModelServer


class LocalModelTests(unittest.TestCase):
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory()
        self.root=Path(self.temp.name).resolve()
        self.server=LocalModelServer(self.root)

    def tearDown(self):
        self.server.close();self.temp.cleanup()

    def assets(self):
        for name in ('runtime/llama/llama-server.exe','models/Qwen3VL-2B-Instruct-Q4_K_M.gguf',
                     'models/mmproj-Qwen3VL-2B-Instruct-Q8_0.gguf'):
            p=self.root/name;p.parent.mkdir(parents=True,exist_ok=True);p.touch()

    def test_existing_server_is_reused_without_starting_or_stopping_it(self):
        self.server.healthy=Mock(return_value=True)
        with patch('roveraid.local_model.subprocess.Popen') as spawn:
            self.assertFalse(self.server.ensure_ready()['started_here'])
            self.server.close();spawn.assert_not_called()

    def test_missing_assets_have_an_actionable_error(self):
        self.server.healthy=Mock(return_value=False)
        with self.assertRaisesRegex(FileNotFoundError,'setup_environment'):
            self.server.ensure_ready()

    def test_cold_start_uses_local_binding_and_model_paths(self):
        self.assets();self.server.healthy=Mock(side_effect=[False,True])
        process=Mock();process.poll.return_value=None
        with patch('roveraid.local_model.subprocess.Popen',return_value=process) as spawn:
            self.assertTrue(self.server.ensure_ready()['started_here'])
            command=spawn.call_args.args[0]
            self.assertEqual(command[command.index('--host')+1],'127.0.0.1')
            self.assertEqual(command[command.index('--port')+1],'8091')
            self.assertIn('--alias',command)

    def test_automatic_start_is_never_attempted_on_remote_host(self):
        self.server.endpoint='http://example.com:8091'
        self.server.healthy=Mock(return_value=False)
        with self.assertRaisesRegex(ValueError,'local HTTP'):
            self.server.ensure_ready()

    def test_failed_server_start_is_reported(self):
        self.assets();self.server.healthy=Mock(return_value=False)
        process=Mock();process.poll.return_value=1
        with patch('roveraid.local_model.subprocess.Popen',return_value=process):
            with self.assertRaisesRegex(RuntimeError,'server exited'):
                self.server.ensure_ready()

    def test_own_process_is_terminated_on_close(self):
        process=Mock();process.poll.return_value=None;self.server.process=process
        self.server.close();process.terminate.assert_called_once()

    def test_existing_server_required_mode_does_not_spawn(self):
        self.server.auto_start=False;self.server.healthy=Mock(return_value=False)
        with patch('roveraid.local_model.subprocess.Popen') as spawn:
            with self.assertRaisesRegex(RuntimeError,'disabled'):self.server.ensure_ready()
            spawn.assert_not_called()

    def test_nvidia_is_selected_instead_of_integrated_vulkan_gpu(self):
        self.assets()
        gpu=self.root/'runtime/llama-vulkan/llama-server.exe';gpu.parent.mkdir(parents=True);gpu.touch()
        devices=Mock(stdout='  Vulkan0: Intel Iris\n  Vulkan1: NVIDIA GeForce RTX 3050\n',stderr='')
        with patch('roveraid.local_model.subprocess.run',return_value=devices):
            command=self.server.command()
        self.assertEqual(command[command.index('--device')+1],'Vulkan1')
        self.assertEqual(command[command.index('-ngl')+1],'99')
        self.assertEqual(self.server.backend,'NVIDIA GPU / Vulkan')

    def test_cpu_is_used_when_vulkan_has_no_nvidia_device(self):
        self.assets()
        gpu=self.root/'runtime/llama-vulkan/llama-server.exe';gpu.parent.mkdir(parents=True);gpu.touch()
        with patch('roveraid.local_model.subprocess.run',return_value=Mock(stdout='Vulkan0: Intel Iris',stderr='')):
            command=self.server.command()
        self.assertEqual(command[command.index('-ngl')+1],'0')
        self.assertNotIn('--device',command)


if __name__=='__main__':unittest.main()
