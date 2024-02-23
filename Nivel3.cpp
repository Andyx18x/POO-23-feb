#include "Nivel3.h"
#include "Game.h"
#include "Menu.h"
#include "Ball.h"
#include "Stats.h"
#include "GameOver.h"
#include "Nivel1.h"
#include "Nivel4.h"
using namespace std;

Nivel3::Nivel3() {
	
	m_ball.IncrementarVelocidad(3.f); 
	m_ball.setBallMoving(false);
	
	m_stats.IncrementarNivel();	
	
	///Cuadros concentricos
	int centerX = columnCount / 2.5;   /// Se calcula la coordenada x del centro de la matriz de bloques
	int centerY = rowCount / 2;   /// Se calcula la coordenada y del centro de la matriz de bloques
	int maxDistance = std::max(centerX, centerY);
	
	for (int i = 0; i < rowCount; ++i) {
		for (int j = 0; j < columnCount; ++j) {
			int distance = std::max(abs(centerX - j), abs(centerY - i));    /// Se calcula la distancia de la posición actual al centro 
			if (distance % 2 == 0) {
				float x = j * (blockWidth + 6.f) + 50.f;
				float y = i * (blockHeight + 6.f) + 5.f;
				bool isSpecial_menospts = (rand () % 10 == 0); /// Probabilidad 1 / 10 de ser especial el bloque puntos (resta 100)
				if(isSpecial_menospts){
					m_blocks.emplace_back(x,y,blockWidth,blockHeight,Color::Blue,false,false,false,true);
					contador_bloques_special++;
				}else{
					m_blocks.emplace_back(x, y, blockWidth, blockHeight, Color::Black);
				}
				
			}
		}
	}
	
	bloques_totales = m_blocks.size();
	
}


void Nivel3::Update(Game &g){
	
	
	if(Keyboard::isKeyPressed(Keyboard::Escape)){
		g.SetScene(new Menu());
		m_ball.setBallMoving(false);
		m_stats.ResetStats();
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Space)){
		m_ball.setBallMoving(true);
		m_stats.CambiarColores();
		m_stats.draw_text(false);
	}
	
	
	if(m_ball.Colisiona(m_player)){
		m_ball.Rebotar(m_player.DimensionesPlayer());
	}
	
	for (auto it = m_blocks.begin(); it != m_blocks.end(); ){
		if (m_ball.Colisiona(*it)) {
			/// Bloque especial de puntos (resta 100)
			if(it->isSpecialPts()){
				m_stats.restarpuntaje(100);
				m_ball.Rebotar();
				it = m_blocks.erase(it); /// Eliminar bloque especial
				continue; /// Continuar con el siguiente bloque
			} 
			
			/// Si no es especial el bloque pasa esto..
			contador_bloques_normales++;
			m_stats.aumentarpuntaje(25);
			m_ball.Rebotar();
			it = m_blocks.erase(it); /// Avanza el iterador después de eliminar el bloque no especial
		} else {
			++it; /// Avanza al siguiente bloque si no hay colisión
		}
	}
	
	
	if(m_ball.falling()){
		m_stats.decrementar_vidas(1);
	}
	
	if(m_stats.VerVidas() == 0){
		m_stats.GuardarScore(m_stats.MostrarPuntajeTotal());
		g.SetScene(new GameOver());
	}
	
	if(m_blocks.empty() or contador_bloques_normales == (bloques_totales - contador_bloques_special)) {
		g.SetScene(new Nivel4());
		
	}
	
	m_stats.actualizarStats();
	m_ball.Update();
	m_player.Update();
	
}

void Nivel3::Draw(RenderWindow &w){
	w.clear({20,20,20});
	m_ball.Draw(w);
	m_player.Draw(w);
	
	m_stats.DrawStats(w);
	
	for(auto &bloque : m_blocks){
		bloque.Draw(w);
	}
}


