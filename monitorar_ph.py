import subprocess
from datetime import datetime, timedelta

# Caminho para o mosquitto_sub
cmd = r'"C:\Program Files\mosquitto\mosquitto_sub.exe" -h localhost -t "pH/#" -v'

# Inicia o processo
processo = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, text=True)

for linha in processo.stdout:
    try:
        partes = linha.strip().split()
        if len(partes) >= 2:
            payload = partes[1]
            campos = payload.split(";")
            if len(campos) >= 2:
                ph = campos[1]
                agora = datetime.utcnow() - timedelta(hours=3)  # UTC-3
                horario_formatado = agora.strftime("%d/%m/%Y %H:%M:%S")
                print(f"[{horario_formatado}] pH: {ph}")
    except Exception as e:
        print("Erro ao processar linha:", e)
