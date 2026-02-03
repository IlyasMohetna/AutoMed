import { useState, useEffect, useRef } from "react";
import { X, Play, Pause, Square, Activity, Terminal as TerminalIcon, ChevronRight } from "lucide-react";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Progress } from "@/components/ui/progress";
import { Badge } from "@/components/ui/badge";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";

const SimulationLargeView = ({ simulationId, onClose, onPause, onResume, onStop }) => {
  const [status, setStatus] = useState(null);
  const [stats, setStats] = useState(null);
  const [events, setEvents] = useState([]);
  const [loading, setLoading] = useState(true);
  const eventLogRef = useRef(null);

  useEffect(() => {
    loadData();
    const interval = setInterval(loadData, 1000);
    return () => clearInterval(interval);
  }, [simulationId]);

  // Auto-scroll to bottom when new events arrive
  useEffect(() => {
    if (eventLogRef.current) {
      eventLogRef.current.scrollTop = eventLogRef.current.scrollHeight;
    }
  }, [events]);

  const loadData = async () => {
    try {
      const ApiService = (await import("@/services/ApiService")).default;
      const [statusData, statsData, eventsData] = await Promise.all([
        ApiService.getSimulationStatus(simulationId),
        ApiService.getSimulationStats(simulationId),
        ApiService.getSimulationEvents(simulationId),
      ]);
      setStatus(statusData);
      setStats(statsData);
      setEvents(eventsData.events || []);
    } catch (error) {
      console.error("Error loading data:", error);
    } finally {
      setLoading(false);
    }
  };

  const getStatusBadge = (etat) => {
    const variants = {
      CREATED: { variant: "secondary", label: "Créée" },
      RUNNING: { variant: "default", label: "En cours" },
      PAUSED: { variant: "outline", label: "En pause" },
      STOPPED: { variant: "destructive", label: "Arrêtée" },
      FINISHED: { variant: "default", label: "Terminée" },
    };
    const { variant, label } = variants[etat] || { variant: "secondary", label: etat };
    return <Badge variant={variant}>{label}</Badge>;
  };

  const progressValue = status?.dureeSimulationMinutes > 0
    ? (status.tempsEcouleMinutes / status.dureeSimulationMinutes) * 100
    : 0;

  const getEventTypeColor = (type) => {
    const colors = {
      ARRIVEE_PATIENT: "text-blue-400",
      DEBUT_OPERATION: "text-emerald-400",
      FIN_OPERATION: "text-purple-400",
      FIN_NETTOYAGE_BLOC: "text-cyan-400",
      ENTREE_SALLE_REVEIL: "text-yellow-400",
      SORTIE_SALLE_REVEIL: "text-orange-400",
      URGENCE: "text-red-400",
      FIN_SIMULATION: "text-slate-400",
    };
    return colors[type] || "text-gray-400";
  };

  return (
    <div className="fixed inset-0 z-50 bg-background">
      {/* Header */}
      <div className="border-b bg-card">
        <div className="container mx-auto px-6 py-4">
          <div className="flex items-center justify-between">
            <div className="flex items-center gap-4">
              <Activity className="h-8 w-8 text-primary" />
              <div>
                <h1 className="text-2xl font-bold">
                  {status?.nom || `Simulation #${simulationId}`}
                </h1>
                <p className="text-sm text-muted-foreground">
                  {status?.algorithme || "N/A"} • ID: {simulationId}
                </p>
              </div>
            </div>
            <div className="flex items-center gap-3">
              {status && getStatusBadge(status.etat)}
              {status?.etat === "RUNNING" && (
                <Button
                  onClick={() => onPause(simulationId)}
                  variant="outline"
                  size="sm"
                >
                  <Pause className="h-4 w-4 mr-2" />
                  Pause
                </Button>
              )}
              {status?.etat === "PAUSED" && (
                <Button
                  onClick={() => onResume(simulationId)}
                  size="sm"
                >
                  <Play className="h-4 w-4 mr-2" />
                  Reprendre
                </Button>
              )}
              {(status?.etat === "RUNNING" || status?.etat === "PAUSED") && (
                <Button
                  onClick={() => onStop(simulationId)}
                  variant="destructive"
                  size="sm"
                >
                  <Square className="h-4 w-4 mr-2" />
                  Arrêter
                </Button>
              )}
              <Button
                onClick={onClose}
                variant="ghost"
                size="icon"
              >
                <X className="h-5 w-5" />
              </Button>
            </div>
          </div>
        </div>
      </div>

      {/* Content - Split View */}
      <div className="h-[calc(100vh-88px)] flex">
        {loading ? (
          <div className="flex-1 flex items-center justify-center">
            <div className="text-center">
              <div className="animate-spin rounded-full h-12 w-12 border-4 border-primary border-t-transparent mx-auto mb-4"></div>
              <p className="text-muted-foreground">Chargement...</p>
            </div>
          </div>
        ) : (
          <>
            {/* Left Panel - Stats and Info */}
            <div className="flex-1 overflow-auto p-6 space-y-6">
              {/* Progress Section */}
              <Card>
                <CardHeader>
                  <CardTitle>Progression de la Simulation</CardTitle>
                </CardHeader>
                <CardContent className="space-y-4">
                  <div className="space-y-2">
                    <div className="flex justify-between text-sm">
                      <span className="text-muted-foreground">Temps écoulé</span>
                      <span className="font-medium">
                        {status?.tempsEcouleMinutes || 0} / {status?.dureeSimulationMinutes || 0} min
                      </span>
                    </div>
                    <Progress value={progressValue} className="h-3" />
                    <div className="text-center text-2xl font-bold text-primary">
                      {progressValue.toFixed(1)}%
                    </div>
                  </div>
                </CardContent>
              </Card>

              {/* Stats Grid */}
              <div className="grid grid-cols-2 gap-4">
                <Card>
                  <CardHeader className="pb-3">
                    <CardTitle className="text-sm font-medium text-muted-foreground">
                      En Attente
                    </CardTitle>
                  </CardHeader>
                  <CardContent>
                    <div className="text-3xl font-bold">
                      {status?.nombrePatientsEnAttente || 0}
                    </div>
                    <p className="text-xs text-muted-foreground mt-1">patients</p>
                  </CardContent>
                </Card>

                <Card>
                  <CardHeader className="pb-3">
                    <CardTitle className="text-sm font-medium text-muted-foreground">
                      En Opération
                    </CardTitle>
                  </CardHeader>
                  <CardContent>
                    <div className="text-3xl font-bold text-primary">
                      {status?.nombrePatientsEnOperation || 0}
                    </div>
                    <p className="text-xs text-muted-foreground mt-1">patients</p>
                  </CardContent>
                </Card>

                <Card>
                  <CardHeader className="pb-3">
                    <CardTitle className="text-sm font-medium text-muted-foreground">
                      En Réveil
                    </CardTitle>
                  </CardHeader>
                  <CardContent>
                    <div className="text-3xl font-bold text-violet-600">
                      {status?.nombrePatientsEnReveil || 0}
                    </div>
                    <p className="text-xs text-muted-foreground mt-1">patients</p>
                  </CardContent>
                </Card>

                <Card>
                  <CardHeader className="pb-3">
                    <CardTitle className="text-sm font-medium text-muted-foreground">
                      Traités
                    </CardTitle>
                  </CardHeader>
                  <CardContent>
                    <div className="text-3xl font-bold text-emerald-600">
                      {status?.nombrePatientsTraites || 0}
                    </div>
                    <p className="text-xs text-muted-foreground mt-1">patients</p>
                  </CardContent>
                </Card>
              </div>

              {/* Resources */}
              <Card>
                <CardHeader>
                  <CardTitle>Ressources</CardTitle>
                </CardHeader>
                <CardContent>
                  <div className="grid grid-cols-3 gap-6 text-center">
                    <div>
                      <div className="text-4xl font-bold text-emerald-600">
                        {status?.nombreBlocsLibres || 0}
                      </div>
                      <p className="text-sm text-muted-foreground mt-2">Blocs Libres</p>
                    </div>
                    <div>
                      <div className="text-4xl font-bold text-red-600">
                        {status?.nombreBlocsOccupes || 0}
                      </div>
                      <p className="text-sm text-muted-foreground mt-2">Blocs Occupés</p>
                    </div>
                    <div>
                      <div className="text-4xl font-bold text-primary">
                        {status?.nombreEquipesDisponibles || 0}
                      </div>
                      <p className="text-sm text-muted-foreground mt-2">Équipes Disponibles</p>
                    </div>
                  </div>
                </CardContent>
              </Card>

              {/* Stats Details */}
              <Card>
                <CardHeader>
                  <CardTitle>Statistiques Détaillées</CardTitle>
                </CardHeader>
                <CardContent>
                  {stats && Object.keys(stats).length > 1 ? (
                    <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                      {Object.entries(stats).map(([key, value]) => {
                        if (key === "simulationId") return null;
                        
                        let displayValue;
                        if (typeof value === "object" && value !== null) {
                          displayValue = JSON.stringify(value, null, 2);
                        } else if (typeof value === "number") {
                          displayValue = value.toFixed(2);
                        } else {
                          displayValue = String(value);
                        }
                        
                        return (
                          <div key={key} className="border rounded-lg p-3">
                            <div className="text-xs text-muted-foreground mb-1">
                              {key.replace(/([A-Z])/g, " $1").trim()}
                            </div>
                            <div className="text-lg font-bold break-words">
                              {displayValue}
                            </div>
                          </div>
                        );
                      })}
                    </div>
                  ) : (
                    <div className="text-center py-8 text-muted-foreground">
                      Aucune statistique disponible
                    </div>
                  )}
                </CardContent>
              </Card>
            </div>

            {/* Right Panel - Live Event Log Terminal */}
            <div className="w-[45%] border-l bg-slate-950 flex flex-col">
              <div className="border-b border-slate-800 px-4 py-3 bg-slate-900">
                <div className="flex items-center gap-2">
                  <TerminalIcon className="h-5 w-5 text-emerald-400" />
                  <h2 className="text-sm font-semibold text-slate-200">Live Event Log</h2>
                  <Badge variant="outline" className="ml-auto text-emerald-400 border-emerald-400">
                    {events.length} events
                  </Badge>
                </div>
              </div>
              
              <div 
                ref={eventLogRef}
                className="flex-1 overflow-y-auto p-4 font-mono text-xs space-y-1"
              >
                {events.length > 0 ? (
                  events.map((event, index) => (
                    <div
                      key={index}
                      className="flex items-start gap-2 hover:bg-slate-900/50 px-2 py-1 rounded transition-colors"
                    >
                      <span className="text-slate-500 shrink-0 w-16">
                        {String(event.timestamp).padStart(6, '0')}m
                      </span>
                      <ChevronRight className={`h-4 w-4 shrink-0 ${getEventTypeColor(event.type)}`} />
                      <span className={`shrink-0 w-32 ${getEventTypeColor(event.type)} font-semibold`}>
                        {event.type}
                      </span>
                      <span className="text-slate-300 flex-1">
                        {event.description}
                      </span>
                    </div>
                  ))
                ) : (
                  <div className="text-center py-12 text-slate-600">
                    <TerminalIcon className="h-12 w-12 mx-auto mb-3 opacity-50" />
                    <p>En attente d'événements...</p>
                  </div>
                )}
              </div>

              {/* Terminal Footer */}
              <div className="border-t border-slate-800 px-4 py-2 bg-slate-900 flex items-center gap-2 text-xs">
                <div className="flex items-center gap-1">
                  <div className="h-2 w-2 rounded-full bg-emerald-500 animate-pulse"></div>
                  <span className="text-slate-400">Live</span>
                </div>
                <div className="text-slate-500">•</div>
                <span className="text-slate-400">Auto-scroll: On</span>
                <div className="text-slate-500">•</div>
                <span className="text-slate-400">Refresh: 1s</span>
              </div>
            </div>
          </>
        )}
      </div>
    </div>
  );
};

export default SimulationLargeView;
