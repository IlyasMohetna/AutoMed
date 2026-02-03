import { useState, useEffect } from "react";
import { Play, Pause, Square, Trash2, Clock, Users, Activity } from "lucide-react";
import { Card, CardContent, CardHeader, CardTitle, CardDescription, CardFooter } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Badge } from "@/components/ui/badge";
import { Progress } from "@/components/ui/progress";

const SimulationCard = ({
  simulation,
  status: initialStatus,
  onStart,
  onPause,
  onResume,
  onStop,
  onDelete,
  onViewLarge,
}) => {
  const [status, setStatus] = useState(initialStatus);
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    setStatus(initialStatus);
  }, [initialStatus]);

  const handleAction = async (action) => {
    setLoading(true);
    try {
      await action();
    } finally {
      setLoading(false);
    }
  };

  const getStatusBadge = (etat) => {
    const configs = {
      CREATED: { variant: "secondary", label: "Créée" },
      RUNNING: { variant: "default", label: "En cours", className: "bg-emerald-500 hover:bg-emerald-600" },
      PAUSED: { variant: "outline", label: "En pause" },
      STOPPED: { variant: "destructive", label: "Arrêtée" },
      FINISHED: { variant: "secondary", label: "Terminée", className: "bg-blue-500 hover:bg-blue-600 text-white" },
    };
    const config = configs[etat] || { variant: "secondary", label: etat };
    return <Badge variant={config.variant} className={config.className}>{config.label}</Badge>;
  };

  const progressValue = status?.dureeSimulationMinutes > 0
    ? (status.tempsEcouleMinutes / status.dureeSimulationMinutes) * 100
    : 0;

  const handleCardClick = (e) => {
    // Don't trigger if clicking on buttons or interactive elements
    if (e.target.closest('button') || e.target.closest('[role="button"]')) {
      return;
    }
    
    // Open large view if simulation is running or paused
    if (onViewLarge && (status?.etat === "RUNNING" || status?.etat === "PAUSED")) {
      onViewLarge(simulation);
    }
  };

  return (
    <Card 
      className={`overflow-hidden hover:shadow-lg transition-shadow ${
        (status?.etat === "RUNNING" || status?.etat === "PAUSED") ? "cursor-pointer" : ""
      }`}
      onClick={handleCardClick}
    >
      <CardHeader className="border-b bg-muted/40">
        <div className="flex items-start justify-between">
          <div className="flex-1">
            <CardTitle className="text-lg flex items-center gap-2">
              <Activity className="h-4 w-4 text-primary" />
              {status?.nom || `Simulation #${simulation}`}
            </CardTitle>
            <CardDescription className="mt-1.5">
              #{simulation} • {status?.algorithme || "N/A"}
            </CardDescription>
          </div>
          {status && getStatusBadge(status.etat)}
        </div>
      </CardHeader>

      <CardContent className="p-6 space-y-6">
        {/* Progress */}
        {status && status.progression !== undefined && (
          <div className="space-y-2">
            <div className="flex items-center justify-between text-sm">
              <span className="text-muted-foreground flex items-center gap-1">
                <Clock className="h-3 w-3" />
                Progression
              </span>
              <span className="font-semibold">{progressValue.toFixed(0)}%</span>
            </div>
            <Progress value={progressValue} className="h-2" />
            <div className="flex justify-between text-xs text-muted-foreground">
              <span>{status.tempsEcouleMinutes || 0} min</span>
              <span>{status.dureeSimulationMinutes || 0} min</span>
            </div>
          </div>
        )}

        {/* Stats Grid */}
        {status && (
          <div className="grid grid-cols-2 gap-3">
            <div className="rounded-lg border bg-card p-3 hover:bg-accent/50 transition-colors">
              <div className="text-2xl font-bold">{status.nombrePatientsEnAttente || 0}</div>
              <div className="text-xs text-muted-foreground mt-1">En Attente</div>
            </div>
            <div className="rounded-lg border bg-card p-3 hover:bg-accent/50 transition-colors">
              <div className="text-2xl font-bold text-primary">{status.nombrePatientsEnOperation || 0}</div>
              <div className="text-xs text-muted-foreground mt-1">En Opération</div>
            </div>
            <div className="rounded-lg border bg-card p-3 hover:bg-accent/50 transition-colors">
              <div className="text-2xl font-bold text-violet-600">{status.nombrePatientsEnReveil || 0}</div>
              <div className="text-xs text-muted-foreground mt-1">En Réveil</div>
            </div>
            <div className="rounded-lg border bg-card p-3 hover:bg-accent/50 transition-colors">
              <div className="text-2xl font-bold text-emerald-600">{status.nombrePatientsTraites || 0}</div>
              <div className="text-xs text-muted-foreground mt-1">Traités</div>
            </div>
          </div>
        )}

        {/* Resources */}
        {status && (
          <div className="flex items-center justify-between text-sm border-t pt-4">
            <div className="flex items-center gap-2">
              <div className="h-2 w-2 rounded-full bg-emerald-500" />
              <span className="text-muted-foreground">Libres:</span>
              <span className="font-semibold">{status.nombreBlocsLibres || 0}</span>
            </div>
            <div className="flex items-center gap-2">
              <div className="h-2 w-2 rounded-full bg-destructive" />
              <span className="text-muted-foreground">Occupés:</span>
              <span className="font-semibold">{status.nombreBlocsOccupes || 0}</span>
            </div>
            <div className="flex items-center gap-2">
              <Users className="h-3 w-3 text-muted-foreground" />
              <span className="font-semibold">{status.nombreEquipesDisponibles || 0}</span>
            </div>
          </div>
        )}
      </CardContent>

      <CardFooter className="flex gap-2 border-t bg-muted/20 p-3">
        {status?.etat === "CREATED" && (
          <Button
            onClick={() => handleAction(() => onStart(simulation))}
            disabled={loading}
            className="flex-1"
            size="sm"
          >
            <Play className="h-4 w-4 mr-1" />
            Démarrer
          </Button>
        )}

        {status?.etat === "RUNNING" && (
          <Button
            onClick={() => handleAction(() => onPause(simulation))}
            disabled={loading}
            variant="outline"
            className="flex-1"
            size="sm"
          >
            <Pause className="h-4 w-4 mr-1" />
            Pause
          </Button>
        )}

        {status?.etat === "PAUSED" && (
          <Button
            onClick={() => handleAction(() => onResume(simulation))}
            disabled={loading}
            className="flex-1"
            size="sm"
          >
            <Play className="h-4 w-4 mr-1" />
            Reprendre
          </Button>
        )}

        {(status?.etat === "RUNNING" || status?.etat === "PAUSED") && (
          <Button
            onClick={() => handleAction(() => onStop(simulation))}
            disabled={loading}
            variant="destructive"
            className="flex-1"
            size="sm"
          >
            <Square className="h-4 w-4 mr-1" />
            Arrêter
          </Button>
        )}

        {(status?.etat === "STOPPED" || status?.etat === "FINISHED" || status?.etat === "CREATED") && (
          <Button
            onClick={() => handleAction(() => onDelete(simulation))}
            disabled={loading}
            variant="ghost"
            size="sm"
          >
            <Trash2 className="h-4 w-4" />
          </Button>
        )}
      </CardFooter>
    </Card>
  );
};

export default SimulationCard;
