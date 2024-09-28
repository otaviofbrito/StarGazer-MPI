.PHONY: all c_project purge clean hist build remove_env

TARGET_PY = python_project/histogram.py
TARGET_MAIN = c_project/main

all: c_project

purge: clean remove_env

env: 
	@if [ ! -d ".env" ]; then \
		echo "[WARNING]: O ambiente virtual não foi criado. Execute 'make build' primeiro!"; \
		exit 1; \
	fi
	
c_project:
	$(MAKE) -C c_project

run: 
	@echo "[INFO] Usage: make run p=<number of process> img=<path/to/image.pgm> th=<threshold value>"
	mpirun -np $(p) $(TARGET_MAIN) ${th} $(img)

hist: env
	@if [ -z "$(img)" ]; then \
		echo "[INFO] Usage: make hist img=<image path>"; \
		exit 1; \
	fi
	. .env/bin/activate; python3 $(TARGET_PY) $(img); deactivate

clean:
	$(MAKE) -C c_project clean
	@echo "[OK]: Ambiente limpo!"

build:
	@echo "[INFO]: Criando ambiente virtual..."
	python3 -m venv .env
	@echo "[INFO]: Instalando dependências no ambiente virtual ..."
	. .env/bin/activate; pip install -r requirements.txt; deactivate
	@echo "[OK]: Ambiente virtual criado!"

remove_env:
	@echo "[INFO]: Excluindo ambiente virtual..."
	rm -rf .env
	@echo "[OK]: Ambiente virtual removido!"


help:
	@echo "Makefile Help:"
	@echo "  all           : Compila o projeto C."
	@echo "  purge         : Limpa o ambiente e remove o ambiente virtual."
	@echo "  env           : Verifica se o ambiente virtual foi criado."
	@echo "  run           : Executa o programa com mpirun."
	@echo "  hist          : Executa o script Python para gerar histogramas."
	@echo "  clean         : Limpa o ambiente de compilação do projeto C."
	@echo "  build         : Cria um ambiente virtual Python e instala dependências."
	@echo "  remove_env    : Remove o ambiente virtual."
	@echo "  help          : Mostra esta ajuda."